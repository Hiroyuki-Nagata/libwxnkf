# -- Top-level Makefile.am --
# Subdirectories to descend into.
SUBDIRS = src test

dist-hook: ChangeLog

ChangeLog: configure.ac
	git log --stat --name-only --date=short --abbrev-commit > ChangeLog
