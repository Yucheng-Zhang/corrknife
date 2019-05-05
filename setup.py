import numpy
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

ext_modules = [Extension(
    name="pc2d",
    sources=["calc.pyx", "pc2d.c"],
    # extra_objects=["fc.o"],  # if you compile fc.cpp separately
    include_dirs=[numpy.get_include()],  # .../site-packages/numpy/core/include
    language="c",
    # libraries=
    extra_compile_args=['-O3'],
    # extra_link_args = "...".)
)]

ext_modules = cythonize(ext_modules, compiler_directives={
                        'language_level': "3"})

setup(
    name='pc2d',
    cmdclass={'build_ext': build_ext},
    ext_modules=ext_modules,
)
