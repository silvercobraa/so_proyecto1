#!/usr/bin/env python
# coding=utf-8

import os
import sys

prompt = "Shell 100% Real No Fake 1 Link >>> "
buff = ""

while (True):
	try:
		buff = input(prompt)
	except EOFError as eofe:
		print("") # se imprime un salto de linea para que la prompt de la shell no quede fea
		exit()
	commands = buff.split("|")
	# print("comandos: " + str(commands))
	previous_pipe = None
	current_pipe = None

	for i, cmd in enumerate(commands):

		if i != len(commands) - 1:
			current_pipe = os.pipe()

		pid = os.fork()
		if pid == 0:
			# caso especial primer proceso
			if i != 0:
				os.dup2(previous_pipe[0], 0)
				# os.close(pipes[i - 1][0])
				# os.close(pipes[i - 1][1])

			# caso especial último proceso
			if i != len(commands) - 1:
				os.dup2(current_pipe[1], 1)
				# os.close(pipes[i][0])
				# os.close(pipes[i][1])

			arguments = cmd.split()
			print("argumentos: " + str(arguments), file=sys.stderr)
			os.execvp(arguments[0], arguments)
		else:
			if i != 0:
				# No se si cerrar los pipes en el proceso hijo es necesario, pero en el padre si
				os.close(previous_pipe[0])
				os.close(previous_pipe[1])
			os.wait()
			print(previous_pipe)
			print(current_pipe)
			previous_pipe = current_pipe
