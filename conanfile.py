from conans import ConanFile

import os

class Taskranger(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "scons"

    default_options = {
        "icu:data_packaging": "archive",
        "icu:shared": False,
        "icu:with_dyload": False
    }

    def requirements(self):
        self.requires("nlohmann_json/3.8.0")
        self.requires("icu/67.1")
