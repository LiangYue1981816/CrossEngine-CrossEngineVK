def ModuleFileList(moduleName, msvcProjectFileName, androidFileName):
	from xml.dom.minidom import parse;
	import xml.dom.minidom;
	
	DOMTree = xml.dom.minidom.parse(msvcProjectFileName);
	collection = DOMTree.documentElement;
	
	file = open(androidFileName, "w");
	file.write("%s := \\\n" % moduleName);
	
	itemGroups = collection.getElementsByTagName("ItemGroup");
	for itemGroup in itemGroups:
		files = itemGroup.getElementsByTagName("ClCompile");
		for index in range(files.length):
			fileName = files[index].getAttribute("Include");
			fileName = fileName.replace("\\", "/");
			if index < files.length - 1:
				fileName += " \\";
			file.write(fileName + "\n");
			
	file.close();
	return;
	
ModuleFileList("MODULE_ENGINE_SRC_FILES", "./msvc/CrossEngine.vcxproj", "./android/jni/MODULE_ENGINE_SRC_FILES.mk");