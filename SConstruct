# -*- mode: python -*-

# Without this ENV snippet, Scons won't find executables in MacPorts'
# installation path, /opt/local/bin.
import os
env = Environment(ENV = {'PATH' : os.environ['PATH']})

# Include the v8 SConstruct file which, unfortunately, automatically builds v8.
# TODO: Improve this.
SConscript('deps/v8/SConstruct', exports=['env'])

# Include the Noble SCons directives.
SConscript('src/SConscript', exports=['env'])

# TODO: Add build target to run cpplint.py
