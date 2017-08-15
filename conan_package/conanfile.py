import os
import shutil
from distutils.dir_util import copy_tree
from base import PaymentChannelBase

# The reason the local package is named conanfile.py and not testing.py is to we can run
# the test_pacakge command (it doesn't accept a --filename arg)
# This way the "default mode" of working with the pacakges is with the testing channels

# By default conan export uses the testing channel if not specified so it is also convenient to just run
# `conan export joystream` for local development

LOCAL_REPO_PATH_ENV = "PAYMENTCHANNEL_LOCAL_REPO_PATH"

class PaymnetChannelTesting(PaymentChannelBase):
    # No real version is defined when working with a package on the testing channel
    # Keep at at 0.0.0 to detect accidentally exporting to stable branch
    # It would be nice to have a method that conan calls when the recipie is being exported
    version = "0.0.0"

    exports = "base.py"

    exports_sources = "../sources*"

    # override base policy
    build_policy="always"

    def source(self):
        repo_path = os.getenv(LOCAL_REPO_PATH_ENV)

        if (repo_path):
            os.makedirs("repo/sources")
            copy_tree("%s/sources" % repo_path, "repo/sources")
        else:
            os.mkdir("repo")
            shutil.move("sources", "repo/")

    def requirements(self):
        self.requires("gtest/1.8.0@lasote/stable")

    def configure(self):
        self.options["gtest"].shared=False
