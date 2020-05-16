PRETTY 		= python $(BASE).dbuild/pretty/pretty.py
PRETTIFY	= python $(BASE).dbuild/pretty/prettify.py
PCP			= python $(BASE).dbuild/pretty/prettycp.py --rbuild "CP"
PMD			= python $(BASE).dbuild/pretty/prettymd.py
PRM			= $(PRETTIFY) --rbuild "RM"
PCHMOD		= python $(BASE).dbuild/pretty/prettychmod.py
PRETTYSAMBA	= python $(BASE).dbuild/pretty/prettysamba.py
PRETTYLINUX = python $(BASE).dbuild/pretty/prettylinux.py
PTODO		= python $(BASE).dbuild/pretty/todo.py

ifeq ($(DBUILD_VERBOSE_CMD), 1)
#PRETTY=@ #
endif

#
#	PRM (Pretty Remove) flags
#
PRM_PIPE=
PRM_FLAGS=-rf

ifeq ($(DBUILD_VERBOSE_CMD), 0)
PRM_PIPE=| $(PRM) $(MODULE_NAME)
PRM_FLAGS=-rvf
endif

#
#	PCP Flags
#
PCP_PIPE=
PCP_FLAGS=-r

ifeq ($(DBUILD_VERBOSE_CMD), 0)
PCP_PIPE=| $(PCP) $(MODULE_NAME)
PCP_FLAGS=-vr
endif

#
# 	PMD Flags
#
PMD_FLAGS=-p
PMD_PIPE=

ifeq ($(DBUILD_VERBOSE_CMD), 0)
PMD_FLAGS=-pv
PMD_PIPE=| $(PMD) $(MODULE_NAME)
endif
