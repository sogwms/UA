import config

env = Environment(ARCOMSTR = 'AR $TARGET',
                  ASCOMSTR = 'AS $TARGET',
                  ASPPCOMSTR = 'AS $TARGET',
                  CCCOMSTR = 'CC $TARGET',
                  CXXCOMSTR = 'CXX $TARGET',
                  LINKCOMSTR = 'LINK $TARGET',
                  CPPDEFINES = config.CPPDEFINES,
                  AS = config.AS, ASFLAGS = config.ASFLAGS,
                  CC = config.CC, CCFLAGS = config.CCFLAGS,
                  AR = config.AR, ARFLAGS = '-rc',
                  CXX = config.CXX, CXXFLAGS = config.CXXFLAGS,
                  LINK = config.LINK, LINKFLAGS = config.LINKFLAGS)

env.PrependENVPath('PATH', config.EXEC_PATH)

Export('env')

objs = SConscript('SConscript', variant_dir='build', duplicate=0)

# objs = SConscript('SConscript')
env.Program(target=config.TARGET, source=objs)

env.AddPostAction(config.TARGET, config.OBJCPY + ' -Oihex ' + config.TARGET + ' ' + config.HEX_TARGET)
env.AddPostAction(config.TARGET, config.OBJCPY + ' -Obinary -S ' + config.TARGET + ' ' + config.BIN_TARGET)
env.AddPostAction(config.TARGET, config.SIZE + ' ' + config.TARGET)

Clean(config.TARGET, config.BIN_TARGET)
Clean(config.TARGET, config.HEX_TARGET)
Clean(config.TARGET, 'build')


Help("""
The fllowing help info just a test, not real
""")

Help("""
Type: 'scons program' to build the production program,
'scons debug' to build the debug version.
""")

