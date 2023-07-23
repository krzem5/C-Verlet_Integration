from PIL import Image
import os
import subprocess
import sys



IMAGE_ALIGNMENT_MASK=3

BYTES_PER_LINE=16



def _get_file_as_string(file_path):
	out="\""
	with open(file_path,"rb") as rf:
		for c in rf.read():
			if (c==10):
				out+="\\n"
			elif (c==9):
				out+="\\t"
			elif (c==13):
				out+="\\r"
			elif (c==34):
				out+="\\\""
			elif (c<32 or c>126):
				out+=f"\\x{c:02x}"
			else:
				out+=chr(c)
	return out+"\""



def load_shaders(source_directory,file_path_header,file_path_source):
	names=[name.split(".")[0] for name in os.listdir(source_directory) if name.endswith(".vs")]
	uniforms={}
	uniform_count=0
	with open(file_path_header,"w") as wf:
		wf.write(f"#ifndef _VERLET_INTEGRATION_GENERATED_SHADERS_H_\n#define _VERLET_INTEGRATION_GENERATED_SHADERS_H_ 1\n\n\n\n#define SHADER_COUNT {len(names)}\n\n")
		for i,name in enumerate(names):
			wf.write(f"#define SHADER_{name.upper()} ((shader_id_t)_compiled_programs[{i}])\n")
			uniforms[name]=[]
			with open(os.path.join(source_directory,name)+".uniforms","r") as rf:
				for uniform in rf.read().split("\n"):
					uniform=uniform.strip()
					if (not uniform):
						continue
					uniforms[name].append((uniform,uniform_count))
					wf.write(f"#define SHADER_{name.upper()}_{uniform.lower()} (_program_uniforms[{uniform_count}])\n")
					uniform_count+=1
		wf.write(f"\n\n\ntypedef struct _SHADER_PROGRAM_SOURCE_UNIFORM{{\n\tconst char* name;\n\tunsigned int offset;\n}} shader_program_source_uniform_t;\n\n\n\ntypedef struct _SHADER_PROGRAM_SOURCE{{\n\tconst char* vertex;\n\tconst char* geometry;\n\tconst char* fragment;\n\tconst shader_program_source_uniform_t* uniforms;\n\tunsigned int uniform_count;\n}} shader_program_source_t;\n\n\n\nextern unsigned int _compiled_programs[{len(names)}];\nextern unsigned int _program_uniforms[{uniform_count}];\nextern const shader_program_source_t _program_sources[{len(names)}];\n\n\n\n#endif\n")
	with open(file_path_source,"w") as wf:
		wf.write(f"#include <verlet_integration/generated/shaders.h>\n\n\n\nunsigned int _compiled_programs[{len(names)}];\nunsigned int _program_uniforms[{uniform_count}];")
		for name,uniform_list in uniforms.items():
			wf.write(f"\n\n\n\nconst shader_program_source_uniform_t _shader_uniforms_{name.lower()}[{len(uniform_list)}]={{")
			for uniform,index in uniform_list:
				wf.write(f"\n\t{{\n\t\t\"{uniform}\",\n\t\t{index}\n\t}},")
			wf.write("\n};")
		wf.write(f"\n\n\n\nconst shader_program_source_t _program_sources[{len(names)}]={{")
		for name in names:
			wf.write(f"\n\t{{\n\t\t{_get_file_as_string(os.path.join(source_directory,name)+'.vs')},\n\t\t{_get_file_as_string(os.path.join(source_directory,name)+'.gs')},\n\t\t{_get_file_as_string(os.path.join(source_directory,name)+'.fs')},\n\t\t{'_shader_uniforms_'+name.lower() if uniforms[name] else 'NULL'},\n\t\t{len(uniforms[name])}\n\t}},")
		wf.write("\n};\n")



if (os.path.exists("build")):
	dl=[]
	for r,ndl,fl in os.walk("build"):
		r=r.replace("\\","/").strip("/")+"/"
		for d in ndl:
			dl.insert(0,r+d)
		for f in fl:
			os.remove(r+f)
	for k in dl:
		os.rmdir(k)
else:
	os.mkdir("build")
load_shaders("rsrc/shader/","src/include/verlet_integration/generated/shaders.h","src/verlet_integration/generated/shaders.c")
if ("--release" in sys.argv):
	fl=[]
	error=False
	for r,_,cfl in os.walk("src"):
		r+="/"
		for f in cfl:
			if (f[-2:]==".c"):
				fl.append(f"build/{(r+f).replace('/','$')}.o")
				if (subprocess.run(["gcc","-Wall","-lm","-Werror","-march=native","-mno-avx256-split-unaligned-load","-ffast-math","-momit-leaf-frame-pointer","-Ofast","-c",r+f,"-o",f"build/{(r+f).replace('/','$')}.o","-Isrc/include","-DNULL=((void*)0)"]).returncode!=0):
					error=True
	if (error or subprocess.run(["gcc","-o","build/verlet_integration"]+fl+["-lm","-lGL","-lglfw"]).returncode!=0):
		sys.exit(1)
else:
	fl=[]
	error=False
	for r,_,cfl in os.walk("src"):
		r+="/"
		for f in cfl:
			if (f[-2:]==".c"):
				fl.append(f"build/{(r+f).replace('/','$')}.o")
				if (subprocess.run(["gcc","-Wall","-lm","-Werror","-march=native","-O0","-g","-c",r+f,"-o",f"build/{(r+f).replace('/','$')}.o","-Isrc/include","-DNULL=((void*)0)"]).returncode!=0):
					error=True
	if (error or subprocess.run(["gcc","-o","build/verlet_integration"]+fl+["-lm","-lGL","-lglfw"]).returncode!=0):
		sys.exit(1)
if ("--run" in sys.argv):
	subprocess.run(["build/verlet_integration"])
