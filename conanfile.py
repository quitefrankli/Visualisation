from conans import ConanFile, CMake

class manatee_conan(ConanFile):
	name = 'Visualisation'
	version = '1.0.0'

	settings = (
		"os",
		"compiler", 
		"build_type",
		"arch"
	)

	options = {
		'enable_console': [True, False]
	}

	default_options = {
		'enable_console': False
	}

	requires = (
	)

	generators = (
		"cmake",
		"cmake_find_package"
	)

	def imports(self):
		# copies all dll to bin folder (win)
		self.copy("*.dll", dst="bin", src="bin")
		# copies all dll to bin folder (macosx)
		self.copy("*.dylib*", dst="bin", src="lib")

	def build(self):
		cmake = CMake(self, build_type=self.settings.build_type)
		def process_option(option):
			if isinstance(option, bool):
				return '1' if option else '0'
			elif isinstance(option, str):
				return option.upper()
			else:
				raise RuntimeError(f'unsupported option type! {option}')
		
		for key, val in self.options.items():
			cmake.definitions[key.upper()] = process_option(val)
		cmake.configure()

		cmake.build(target='VisualisationLibs')
		cmake.build(target='VisualisationDisplay')

	def package(self):
		self.copy("*.lib", src="lib", dst="lib")
		self.copy("*.hpp", src=f"{self.source_folder}/VisualisationLib", dst="include")
		self.copy("bin/*.exe")
		self.copy("bin/*.dll")
		return super().package()

	def export_sources(self):
		self.output.info("Executing export_sources() method")
		self.copy("CMakeLists.txt")
		self.copy("VisualisationLib/*")
		self.copy("Display/*")