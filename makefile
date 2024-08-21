SHELL := /bin/bash
.ONESHELL:

INCLUDE_FILES_EXTENTIONS := .hpp .tpp .inl .h
OBJ_FILE_EXTENSION := .o
SOURCE_FILES_EXTENTIONS := .cpp .cc

#functions
define are_equal
ifeq ($(1),$(2))
1
endif
endef
define name_to_target
ifeq ($(TEMP_TARGET),STATIC)
$(BUILD_DIR)/lib$(1).a
else ifeq ($(TEMP_TARGET),SHARED)
$(BUILD_DIR)/nothing_$(1).lib
else ifeq ($(TEMP_TARGET),DYNAMIC)
$(BUILD_DIR)/nothing_$(1).lib
else # executable
$(BUILD_DIR)/$(1).exe
endif
endef
pos_last_impl = $(if $(findstring $(1),$(2)),\
					 			$(call pos_last_impl,$(1),$(wordlist 2,$(words $(2)),$(2)),x $(3)),\
					 			$(3))
pos_last = $(words $(call pos_last_impl,$(1),$(2)))
pos_first_impl = $(intcmp $(words $(filter $(1),$(2))),0,,\
								 $(3),\
								 $(call pos_first_impl,$(1),$(wordlist 2,$(words $(2)),$(2)), x $(3)))
pos_first = $(words $(call pos_first_impl,$(1),$(2)))
src_to_obj = $(OBJ_DIR)/$(1)$(OBJ_FILE_EXTENSION)
get_dirs_path = $(patsubst %/,%,$(filter %/,$(wildcard $(1)/*/)))
get_files_path_impl = $(filter-out $(call get_dirs_path,$(1)),\
											$(foreach ext,$(SOURCE_FILES_EXTENTIONS),$(wildcard $(1)/*$(ext))))
get_files_path = $(if $(1),\
								 $(foreach path,$(1),\
								 $(patsubst $(CURDIR)/%,%,$(call get_files_path_impl,$(path)))))
get_files_recur_impl = $(call get_files_path_impl,$(1))\
											 $(foreach dir,\
											 $(filter-out $(IGNORE_DIRS),$(call get_dirs_path,$(1))),\
											 $(call get_files_recur_impl,$(dir)))
get_files_recur = $(if $(1),\
									$(foreach path,$(1),\
									$(foreach file,\
									$(call get_files_recur_impl,$(patsubst $(CURDIR)/%,%,$(path))),\
									$(shell mkdir -p $(BUILD_DIR)/$(dir $(file))) $(file))),\
									$(foreach file,\
									$(call get_files_recur_impl,$(CURDIR)),\
									$(shell mkdir -p $(BUILD_DIR)/$(dir $(file))) $(file)))
make_unique = $(if $(1),\
							$(firstword $(1)),\
							$(call make_unique,$(filter-out $(firstword $(1)),$(1))))

include makefile.var

# Protection phase
# Need this to not leave an empty spot
#EXECUTE_AFTER_COMPILE := $(if $(EXECUTE_AFTER_COMPILE),true)

# 0 - equal names and targets
# 1 - more names than targets
# 2 - more targets than names
NAMES_TARGETS_DIF := $(intcmp $(words NAMES),$(words TARGETS),2,0,1)
ifeq ($(NAMES_TARGETS_DIF),2)
$(error More targets than names specified)
else ifeq ($(NAMES_TARGETS_DIF),2)
$(info More names than targets specified. They will be treated as executables)
endif

$(foreach lhs,$(NAMES),\
$(foreach rhs,$(wordlist $(call pos_first,$(lhs),x $(NAMES)),$(words $(NAMES)),$(NAMES)),\
$(if $(call are_equal,$(lhs),$(rhs)),\
$(error Same names in names list detected: $(lhs)))))

ifeq ($(MODE),DEBUG)
FLAGS += $(DEBUG_FLAGS)
else ifeq ($(MODE),RELEASE)
FLAGS += $(RELEASE_FLAGS)
else ifeq ($(MODE),MINSIZEREL)
FLAGS += $(MINSIZEREL_FLAGS_FLAGS)
else ifeq ($(MODE),RELWITHDEBINFO)
FLAGS += $(RELWITHDEBINFO_FLAGS)
else
$(error Undefined mode: $(MODE))
endif

define valid_target
ifeq ($(1),EXECUTABLE)
1
else ifeq ($(1),EXE)
1
else ifeq ($(1),STATIC)
1
else ifeq ($(1),DYNAMIC)
1
else ifeq ($(1),SHARED)
1
else
endif
endef
$(foreach target,$(TARGETS),$(if $(call valid_target,$(target)),,$(error Invalid target: $(target))))

$(if $(findstring -c,$(OBJ_ONLY_FLAGS),,$(info OBJ_ONLY_FLAGS doesn't contain -c flag)))



ifdef PROGRESS
print = echo $(1)
time_it = time --format="$(1)" $(2)
print_total_time = $(call print,Compiled in total of $$(($$(date +%s) - $(START_TIME))) seconds!)
else
print = true
time_it = $(2)
print_total_time = true
endif

OBJS := $(foreach file,$(SOURCES),$(call src_to_obj,$(file)))
INCLUDES := $(foreach dir,$(INCLUDE_DIRS),$(addprefix -I,$(dir)))
START_TIME := $(shell date +%s)

# All
all: $(foreach name,$(NAMES),$(call name_to_target,$(name),$(word $(call pos_last,$(name),$(NAMES)),$(TARGETS))))

precompile_header: $(ARGS)
	$(MAKE_SILENT)$(call time_it,Compiled $(ARGS) in %E,$(COMPILER) $(ARGS) -c $(addsufix .gch,$(ARGS)) $(FLAGS));
	$(call print,Compiled in total of $$(($$(date +%s) - $(START_TIME))) seconds!)

define obj_define
$(shell $(COMPILER) -MM $(1) -MT $(call src_to_obj,$(1)) $(INCLUDES) $(foreach dir,$($(2)_INCLUDE_DIRS),$(addprefix -I,$(dir))))\
		$(MAKEFILE_LIST)\
		$(ADDITIONAL_DEPENDENCIES)\
		$($(2)_ADDITIONAL_DEPENDENCIES)
	$(MAKE_SILENT)$(call time_it,Compiled $(notdir $$@) in %E,$(COMPILER) $(OBJ_ONLY_FLAGS) $$< -o $$@ $(FLAGS) $(INCLUDES) $(LIBS));
endef

define target_define
$(1)_OBJS := $(foreach file,$($(1)_SOURCES),$(addsufix $(OBJ_DIR)/,$(file)))
TEMP_TARGET := $(word $(call pos_last,$(1),$(NAMES)),$(TARGETS))
ifeq ($(TEMP_TARGET),STATIC)
$(BUILD_DIR)/lib$(1).a: $(OBJS) $($(1)_OBJS)
	$(MAKE_SILENT)$(call time_it,Compiled $$@ in %E,ar rvs $(BUILD_DIR)/$$@ $(OBJS) $($(1)_OBJS))
else ifeq ($(TEMP_TARGET),SHARED)
$(BUILD_DIR)/nothing_$(1).lib: $(OBJS) $($(1)_OBJS)
	$(MAKE_SILENT)$(call time_it,Compiled $$@ in %E,echo "Dynamic libraries aren't supported yet")
else ifeq ($(TEMP_TARGET),DYNAMIC)
$(BUILD_DIR)/nothing_$(1).lib: $(OBJS) $($(1)_OBJS)
	$(MAKE_SILENT)$(call time_it,Compiled $$@ in %E,echo "Shared libraries aren't supported yet")
else # executable
$(BUILD_DIR)/$(1).exe: $(OBJS) $($(1)_OBJS)
	$(MAKE_SILENT)$(call time_it,Compiled $$@ in %E,$(COMPILER) $(LINK_ONLY_FLAGS) $($(1)_$(LINK_ONLY_FLAGS)) $(FLAGS) $($(1)_FLAGS) $(OBJS) $($(1)_OBJS) -o $$@)
endif

$(foreach file,$($(1)_SOURCES),$(eval $(call obj_define,$(file),$(1))))
endef

$(foreach name,$(NAMES),$(eval $(call target_define,$(name))))
$(foreach file,$(SOURCES),$(eval $(call obj_define,$(file))))



# Cleaning
clean_obj:
	$(MAKE_SILENT)rm -rf $(OBJS) $(foreach name,$(NAMES),$($(name)_OBJS))

clean: clean_obj
	$(MAKE_SILENT)rm -rf $(foreach name,$(NAMES),$(call name_to_target,$(name)))