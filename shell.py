#!/usr/bin/env python
# coding=utf-8

import os
import sys

MENSAJE_INICIAL = "Microsoft Windows [Version 10.0.10240]\n(c) 2015 Microsoft Windows Corporation. All rights reserved.\n"
PROMPT = "C:\\WINDOWS\\system32> "

print(MENSAJE_INICIAL)

while (True):
	try:
		buff = input(PROMPT)
	except EOFError as eofe:
		# se imprime un salto de linea para que la prompt de la shell no quede fea
		print("")
		exit()

	commands = buff.split("|")
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

			# caso especial último proceso
			if i != len(commands) - 1:
				os.dup2(current_pipe[1], 1)

			arguments = cmd.split()
			os.execvp(arguments[0], arguments)
		else:
			if i != 0:
				# No se si cerrar los pipes en el proceso hijo es necesario, pero en el padre si
				os.close(previous_pipe[0])
				os.close(previous_pipe[1])
			os.wait()
			previous_pipe = current_pipe
