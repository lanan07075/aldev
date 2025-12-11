# ****************************************************************************
# CUI
#
# The Advanced Framework for Simulation, Integration, and Modeling (AFSIM)
#
# Copyright (C) 2022 Stellar Science; U.S. Government has Unlimited Rights.
#
# The use, dissemination or disclosure of data in this file is subject to
# limitation or restriction. See accompanying README and LICENSE for details.
# ****************************************************************************

project(artificer)
include(swdev_project)

add_subdirectory(source/lib)
add_subdirectory(source/cli)

install_sources_all_files(. tools/${PROJECT_NAME})
