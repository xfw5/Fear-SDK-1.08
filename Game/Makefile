
###############################################################
#
# Makefile for the Linux game server shared library.
#
# (c) 2006 Monolith Productions, Inc. All Rights Reserved.
#
###############################################################

Game_ServerShell: Game_Shared_Server
		@(echo 'Compiling Game_ServerShell...')
		@(cd ObjectDLL;make)

Game_Shared_Server:
		@(echo 'Compiling Game_Shared_Server...')
		@(cd Shared;make)

clean:
	@(if [ -e Shared/Makefile ]; then make -C Shared clean; fi)
	@(if [ -e ObjectDLL/Makefile ]; then make -C ObjectDLL clean; fi)

