from conans import ConanFile

import os

class Taskranger_Test(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "scons"

    def requirements(self):
        self.requires("catch2/3.0.0@catchorg/stable")
