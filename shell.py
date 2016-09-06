#!/usr/bin/env python
# coding=utf-8

import os
import sys
# import subprocess

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
	print("pipes: " + str(pipes))
	# old_pipe = None

	for i, command in enumerate(commands):

		if i != len(commands) - 1:
			pipes[i] = os.pipe()

		pid = os.fork()
		if pid == 0:
			if i != 0:
				os.dup2(pipes[i - 1][0], 0)
				# os.close(pipes[i - 1][0])
				# os.close(pipes[i - 1][1])
			if i != len(commands) - 1:
				os.dup2(pipes[i][1], 1)
				# os.close(pipes[i][0])
				# os.close(pipes[i][1])

			arguments = command.split()
			# arguments = command.split()
			print("argumentos: " + str(arguments), file=sys.stderr)
			os.execvp(arguments[0], arguments)
		else:
			if i != 0:
				# No se si cerrar los pipes en el proceso hijo es necesario, pero en el padre si
				os.close(pipes[i - 1][0])
				os.close(pipes[i - 1][1])
			os.wait()
