# All project makefiles must define:
# ProjectName
# RootPath
# Objects
# Executable

Version = 0
EXTRAFLAGS=

# Project-dependant settings {
ifeq ($(Executable),true)
	InstallDir = $(RootPath)/bin
	OutputName = $(ProjectName)
else
	InstallDir = $(RootPath)/lib
	OutputShortName = lib$(ProjectName).so
	OutputName = $(OutputShortName).$(Version)
	EXTRAFLAGS += -shared 
endif
BuildDir = $(RootPath)/build/$(ProjectName)
Includes = -I$(RootPath)/include
Objects = $(patsubst %.cpp, $(BuildDir)/%.o, $(Sources))
# end project-dependent settings }


# Tool settings {
CXXFLAGS  = -std=c++14
CXXFLAGS += -fPIC
CXXFLAGS += -fno-exceptions
CXXFLAGS += -fvisibility=default
CXXFLAGS += -fdiagnostics-color=auto
CXXFLAGS_DEBUG   = -g -DDEBUG -D_DEBUG
CXXFLAGS_RELEASE = -O3 -DNDEBUG

CCFLAGS  = -std=c11
CPPFLAGS = $(Defines) $(Includes)
LDFLAGS  =  -Wl,-rpath-link,../../lib,-R,'$$ORIGIN/../lib' -L../../lib $(Libraries)
# end tool settings }

all: debug

.PHONY:
debug: CXXFLAGS+=$(CXXFLAGS_DEBUG)
debug: Build

.PHONY: release  
release: CXXFLAGS+=$(CXXFLAGS_RELEASE)
release: Build

$(BuildDir)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

Build: $(Objects)
	$(CXX) $(EXTRAFLAGS) -o $(BuildDir)/$(OutputName) \
	$(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(Objects)
	cp $(BuildDir)/$(OutputName) $(InstallDir)/$(OutputName)
ifeq ($(Executable),false)
	ln -sf $(OutputName) $(InstallDir)/$(OutputShortName)
endif

.PHONY : clean
clean:
	-rm $(Objects) $(BuildDir)/$(OutputName)
