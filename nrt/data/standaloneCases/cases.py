# -*- coding: utf-8 -*-

# Copyright (c) 2015-2019, RTE (http://www.rte-france.com)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

import os

test_cases = []
standardReturnCode = [0]
standardReturnCodeType = "ALLOWED"

##############################################
#             GoverProportional              #
##############################################

case_name = "Model GoverProportional"
case_description = "GoverProportional"
job_file = os.path.join(os.path.dirname(__file__), "Governors/GoverProportional", "GoverProportional.jobs")

test_cases.append((case_name, case_description, job_file, 32, standardReturnCodeType, standardReturnCode))

##############################################
#            Small Case + Governor           #
##############################################

case_name = "Small Case + Governor"
case_description = "Check synchronous generator response to a load disconnection on a small network"
job_file = os.path.join(os.path.dirname(__file__), "SmallCase+Governor", "SmallCase.jobs")

test_cases.append((case_name, case_description, job_file, 60, standardReturnCodeType, standardReturnCode))
