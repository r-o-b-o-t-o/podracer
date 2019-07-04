 # -*- coding: Utf-8 -*-
from queue import Queue
from threading import Thread
import os
import sys
import time
import subprocess
from inspect import currentframe, getframeinfo
debug = False
display = False
timeout = 100



class Program(object):
    def __init__(self, name, command):
        if debug: print("Create Program '%s' with command '%s'"%(name, command))
        self.queue = Queue()
        self.name = name
        self.params = command.split()
        self.start()

    def start(self):
        def target():
            print(self.params)
            self.process = subprocess.Popen(
                self.params,
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=sys.stderr,
                cwd=os.path.dirname(os.path.realpath(self.params[0])) ,
                universal_newlines=True,
                bufsize=1)
            if debug: print(self.name, "start reading", sep=" : ")
            current_label = None
            content = ""
            for line in self.process.stdout:
                if debug: print("<"+self.name, line, sep=" : ",end="")
                if current_label:
                    if line.startswith("START"):
                        print(self.name,": current label not completed (STOP expected)", file=sys.stderr)
                        break
                    elif line.startswith("STOP"):
                        label =  line.strip().split(maxsplit=1)
                        if len(label) == 1 or label[1] == current_label:
                            if debug: print("QUEUE",self.name,(current_label, content))

                            self.queue.put((current_label, content))
                            current_label = None
                            content = ""
                        else :
                            print(self.name,": 'STOP' label not corresponding to '%s'"%current_label, file=sys.stderr)
                            break
                    else :
                        content+=line
                elif line.startswith("START"):
                    label =  line.strip().split(maxsplit=1)
                    if len(label) == 2:
                        current_label = label[1]
                    else:
                        print(self.name,": 'START' must be followed by a label", file=sys.stderr)
                        break
                else :
                    print(self.name," : no label started (START expected)", file=sys.stderr)
                    break
            if debug:
                print(self.name, "stop reading", sep=" : ")


            self.stop()

        self.thread = Thread(target=target)
        self.thread.start()
        if debug: print("'%s' started"%self.name)
        time.sleep(timeout/1000)

    def stop(self):
        try:
            self.process.terminate()
            self.thread.join()
        except:
            pass
        self.process = None
        self.thread = None
        if debug: print("'%s' stoped"%self.name)

    def is_running(self):
        return self.process != None



    def read(self, label,error_intolerant=False):
        if debug: print("'%s' : read label '%s'"%(self.name, label))
        while not self.queue.empty():
            item = self.queue.get()
            if debug: print("DEQUEUE",self.name,item)

            if item[0] == label:
                if debug: print(item[1],end="")
                return item[1]
            elif error_intolerant:
                print(self.name," : expected label '%s' instead of '%s'"%(label, item[0]), file=sys.stderr)
                self.stop()
                break
        return None

    def write(self, label, content):
        if not self.is_running():
            return
        if content == None:
            content = ""
        if debug: print(">"+self.name, "START %s"%label, sep=" : ")
        self.process.stdin.write("START %s\n"%label)
        for line in content.strip().split("\n"):
            if not line:continue
            if debug:
                print(">"+self.name,line, sep=" : ")
            self.process.stdin.write(line+"\n")

        self.process.stdin.write("STOP %s\n"%label)
        if debug: print(">"+self.name, "STOP %s"%label, sep=" : ")



class PlayerProgram(Program):
	
    def __init__(self, command, number):
        Program.__init__(self, "Player %d"%number, command)
        self.number = number
        self.write("player", str(number))

    def read_action(self, turn):
        return self.read("action", True)

    def write_turn(self, turn, content):
        self.write("turn", content)



class GameEngineProgram(Program):
    def __init__(self, command, players):
        Program.__init__(self, "GameEngine", command)
        self.write("players", str(players))

    def read_turn(self, turn, player):
        instructions = self.read("turn %d %d"%(turn, player), True)
        while not instructions and self.is_running():
            time.sleep(timeout / 1000)
            instructions = self.read("turn %d %d"%(turn, player), True)

        if debug or display:
            print("-" * 80)
            print(instructions)
        return instructions

    def read_settings(self):
        if debug: print(self.name, "READ SETTINGS", sep=" : ")

        instructions = self.read("settings", True)
        while instructions == None and self.is_running():
            time.sleep(timeout/100)
            instructions = self.read("settings", True)

        print("=" * 80)
        print(instructions)
        if debug: print(self.name, "READ SETTINGS DONE", sep=" : ")

        return instructions


    def write_actions(self, turn, player, actions):
        if not actions:
            actions = "NOACTION"
        self.write("actions %d %d"%(turn,player), actions)



class WindowProgram(Program):
    def __init__(self, command):
        Program.__init__(self, "Window", command)

    def write_turn(self, content):
        self.write("turn", content)



fails = 0
winner = -1
if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("configFile name required in first argument")
        quit()
    game_engine = None
    players = []
    debug = "debug" in map(str.lower, sys.argv[1:])
    display = "display" in map(str.lower, sys.argv[1:])


    with open(sys.argv[1]) as config:
        game_engine_command = config.readline().strip()
        window_command = config.readline().strip()
        players_commands = [line.strip() for line in config]
        players = [PlayerProgram(cmd, i) for i, cmd in enumerate(players_commands, 1)]
        game_engine = GameEngineProgram(game_engine_command, len(players_commands))
        window = WindowProgram(window_command)
        settings = game_engine.read_settings()
        if debug:
            print("SETTINGS :", settings, file=sys.stderr)
        window.write("settings", settings)
        window.write("players", str(len(players_commands)))
        for p in players:
            p.write("settings", settings)
    turn = 1

    while game_engine.is_running():
        for nb,p in enumerate(players, 1):
            turn_instructions = game_engine.read_turn(turn, nb)
            if display:
                print(turn)
            if nb == 1:
                window.write_turn(turn_instructions)
            p.write_turn(turn, turn_instructions)
            player_action = p.read_action(turn)
            if display:
                print(nb,player_action)
            if not player_action:
                fails += 1
                if fails > 10:
                    break
            game_engine.write_actions(turn, nb, player_action)
        turn += 1

    for p in players:
        p.stop()
    print(winner)
