#!/usr/bin/env python
# coding=utf-8

import os
import subprocess

prompt = "Shell 100% Real No Fake 1 Link >>> "
buff = ""

while (True):
	try:
		buff = input(prompt)
	except EOFError as eofe:
		print("") # se imprime un salto de linea para que la prompt de la shell no quede fea
		exit()
	commands = buff.split("|")
	pipes = [None for c in range(len(commands) - 1)]
	print("comandos: " + str(commands))
	# print("pipes: " + str(pipes))

	for command in commands:

		# if i != len(commands) - 1:
		# 	pipes[i] = os.pipe()

		pid = os.fork()
		if pid == 0:
			# if i != 0:
			# 	os.dup2(pipes[i - 1][0], 0)
			# if i != len(commands) - 1:
			# 	os.dup2(pipes[i][1], 1)

			# arguments = commands[i].split()
			arguments = command.split()
			print("argumentos: " + str(arguments))
			# ejecutar comando
			os.execvp(arguments[0], arguments)
		else:
			os.wait()

	# os.wait()
# cmd1 = ["ls", "-l"]
# cmd2 = ["wc"]
# p = os.pipe()
# pid = os.fork()
# if pid == 0:
# 	os.dup2(p[1], 1)
# 	os.execvp(cmd1[0], cmd1)
# else:
# 	os.dup2(p[0], 0)
# 	os.execvp(cmd2[0], cmd2)
