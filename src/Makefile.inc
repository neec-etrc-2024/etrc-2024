USE_RASPIKE_ART=1

COPTS += 

SRCLANG := c++

mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

$(warning mkfile_path = $(mkfile_path))

APPL_DIRS := $(APPL_DIRS) $(shell find "$(mkfile_path)" -type d)

$(warning APPL_DIRS = $(APPL_DIRS))

APPL_CXXOBJS := $(APPL_CXXOBJS) $(shell find  $(mkfile_path) ! -name 'app.c*' -type f -name "*.cpp" | sed 's!^.*/!!' | sed 's/\.cpp/\.o/')
$(warning APPL_CXXOBJS = $(APPL_CXXOBJS))

APPL_LIBS += -lm -L $(mkfile_path) -lspike


INCLUDES += -I $(mkfile_path)../../spike-api-cpp/include \
            -I $(mkfile_path)

