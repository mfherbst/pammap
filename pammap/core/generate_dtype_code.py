#!/usr/bin/env python3
## vi: tabstop=4 shiftwidth=4 softtabstop=4 expandtab
## ---------------------------------------------------------------------
##
## Copyright (C) 2018 by Michael F. Herbst and contributors
##
## This file is part of pammap.
##
## pammap is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published
## by the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## pammap is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with pammap. If not, see <http://www.gnu.org/licenses/>.
##
## ---------------------------------------------------------------------

from datetime import date
import os
import sys
import constants


def clean_block(blocktext):
    """
    Strip exactly for whitespaces and leading and trailling newlines

    Returns:
        list of lines
    """
    ret = []
    for line in blocktext.split("\n"):
        if line[:4] == "    ":
            ret.append(line[4:].rstrip())
        else:
            ret.append(line.rstrip())
    if ret[0] == "":
        ret = ret[1:]
    if ret[-1] == "":
        ret = ret[:-1]
    return ret


def licence_header_cpp():
    ret = """
    //
    // Copyright (C) {0:} by Michael F. Herbst and contributors
    //
    // This file is part of pammap.
    //
    // pammap is free software: you can redistribute it and/or modify
    // it under the terms of the GNU Lesser General Public License as published
    // by the Free Software Foundation, either version 3 of the License, or
    // (at your option) any later version.
    //
    // pammap is distributed in the hope that it will be useful,
    // but WITHOUT ANY WARRANTY; without even the implied warranty of
    // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    // GNU Lesser General Public License for more details.
    //
    // You should have received a copy of the GNU Lesser General Public License
    // along with pammap. If not, see <http://www.gnu.org/licenses/>.
    //

    //
    // Do not edit. This file has been automatically generated by the script
    // {1:}
    // Instead edit the script and rerun it.
    //
    """.format(date.today().year, os.path.basename(__file__))
    return clean_block(ret)


NAMESPACE_OPEN = [
    "",
    "namespace pammap {",
    "",
]


NAMESPACE_CLOSE = [
    "",
    "} // namespace pammap",
    "",
]


def to_cpp_type(dtype, full=False):
    """Convert the dtype to the C++ name used for it"""
    if full:
        return "pammap::" + dtype[0].upper() + dtype[1:]
    else:
        return dtype[0].upper() + dtype[1:]


def to_cpp_blocktype(dtype, full=False):
    if full:
        return "pammap::DataBlock<" + to_cpp_type(dtype, full=True) + ">"
    else:
        return "DataBlock<" + to_cpp_type(dtype, full=False) + ">"


def make_supported_cpp_types(dtypes):
    """Convert the dtypes to cpp_types using to_cpp_type
       and also build derived types like DataBlock<ccptype>
       and return the full lot as a list.
    """
    supported_types = [to_cpp_type(dtype) for dtype in constants.cpp.underlying_type]
    supported_types += [
        "DataBlock<" + cpptype + ">" for cpptype in supported_types
    ]
    return supported_types


def generate_typedefs(dtypes):
    """
    Generate the typedefs, which map the C++ names
    for the types to the underlying C++ types.
    """

    output = licence_header_cpp()
    for hdr in constants.cpp.headers:
        output.append("#include " + hdr)

    output += [
        "",
        "// Typedefs mapping the dtype names to their underlying C++ types",
        ""
    ]

    output += NAMESPACE_OPEN
    for dtype in dtypes:
        underlying_type = constants.cpp.underlying_type[dtype]
        output += ["typedef " + underlying_type + " " + to_cpp_type(dtype) + ";"]
    output += NAMESPACE_CLOSE
    return "\n".join(output)


def generate_data_block_instantiation(dtypes):
    output = licence_header_cpp()
    output += ["#include \"typedefs.hxx\""]

    output += NAMESPACE_OPEN
    for dtype in dtypes:
        output.append("template class DataBlock<{0:}>;".format(to_cpp_type(dtype)))
    output += NAMESPACE_CLOSE

    return "\n".join(output)


def generate_pammap_instantiation(dtypes):
    output = licence_header_cpp()
    output += ["#include \"typedefs.hxx\""]

    output += NAMESPACE_OPEN
    for dtype in dtypes:
        output.append("template const {0:}& PamMap::at<{0:}>("
                      "const std::string& key, const {0:}& default_value) "
                      "const;".format(to_cpp_type(dtype)))
        output.append("template {0:}& PamMap::at<{0:}>(const std::string& key, "
                      "{0:}& default_value);".format(to_cpp_type(dtype)))
    output += NAMESPACE_CLOSE

    return "\n".join(output)


def generate_is_supported_type(dtypes):
    output = licence_header_cpp()
    output += ["#include <type_traits>"]
    output += ["#include \"typedefs.hxx\""]

    output += NAMESPACE_OPEN
    output += [
        "/** Is the type T supported by pammap for storage. */",
        "template<typename T>",
        "struct IsSupportedType : public std::false_type {};",
        "",
    ]

    for cpptype in make_supported_cpp_types(dtypes):
        output += [
            "/** Specialisation of IsSupportedType<T> for " + cpptype + ".*/",
            "template<>",
            "struct IsSupportedType<" + cpptype + "> : public std::true_type {};",
            "",
        ]

    output += NAMESPACE_CLOSE
    return "\n".join(output)


def generate_pammap_value(dtypes):
    output = licence_header_cpp()
    output += [
        r"#pragma once",
        r'#include "DataBlock.hpp"',
        r'#include "IsSupportedType.hxx"',
        r'#include "any.hpp"',
        r'#include "typedefs.hxx"',
    ]
    output += NAMESPACE_OPEN

    # Add class header
    output += clean_block(r"""
    /** \brief Class to contain an entry value in a PamMap.
        Essentially a slightly specialised pammap::any */
    class PamMapValue : public any {
     public:
    """)

    # Add fallback constructors
    output += clean_block(r"""
      PamMapValue() = default;

      /** Catch-all constructor, which defaults to an error */
      template <typename ValueType>
      PamMapValue(ValueType) : PamMapValue() {
        static_assert(!std::is_unsigned<ValueType>::value,
                      "Unsigned integer types are not supported with PamMap. "
                      "Use a signed type instead.");

        static_assert(IsSupportedType<ValueType>::value,
                      "This value type is not supported by PamMap.");
      }

      /** Catch-all constructor for std::vector */
      template <typename ValueType>
      PamMapValue(std::vector<ValueType>) : PamMapValue() {
        static_assert(IsSupportedType<ValueType>::value,
                      "Cannot assign a list/array of values as a std::vector "
                      "with PamMap. Use the low-level DataBlock<T> for this purpose.");
      }
    """)
    output.append("")

    # Auto-generate constructors
    for cpptype in make_supported_cpp_types(dtypes):
        output += [
            "  /** Construction from " + cpptype + " */",
            "  PamMapValue(" + cpptype + " val) : any(std::move(val)) {}",
            ""
        ]

    # Add transforming constructors
    output += clean_block(r"""
      /** \brief Make a PamMapValue out of an initialiser list by conversion to
                 a DataBlock of the appropriate type. */
      template <typename T>
      PamMapValue(const std::initializer_list<T> il) : PamMapValue(DataBlock(il)) {
        static_assert(IsSupportedType<DataBlock<T>>::value,
                      "The chosen type is not supported for list elements with PamMap.");
      }

      //
      // The int type gets special treatment because it is the default for raw
      // numbers, such that simple things like PamMapValue{1,2,3} just work.
      //
      /** \brief Make an PamMapValue out of an int. Behaves like a PamMapValue
        *        containing an INTEGER type */
      PamMapValue(int i) : PamMapValue(static_cast<Integer>(i)) {}

      /** \brief Make a PamMapValue out of an initialiser list of int */
      PamMapValue(std::initializer_list<int> il)
            : PamMapValue(DataBlock<Integer>(il.begin(), il.end())) {}

      //
      // Same for const char*
      //
      /** \brief Make an PamMapValue out of a const char*.
        * This behaves like the equivalent GenMapValue of a  std::string */
      PamMapValue(const char* s) : PamMapValue(std::string(s)) {}
    """)
    output.append("")

    # Add type_name() method
    output += clean_block(r"""
      /** Return the demangled typename of the type of the internal object. */
      std::string type_name() const;
    """)

    # Close class and namespace
    output += ["};"]
    output += NAMESPACE_CLOSE
    return "\n".join(output)


def generate_data_block_i(dtypes):
    with open("templates/DataBlock.i.template") as f:
        original = f.readlines()

    output = [""]
    for dtype in dtypes:
        if dtype not in constants.python.underlying_numpy_type:
            continue
        nptype = constants.python.underlying_numpy_type[dtype]
        output += [
            "%datablock_typemaps(" + to_cpp_blocktype(dtype, full=True) +
            ", " + nptype + ")"
        ]
    return "".join(original) + "\n".join(output)


def generate_pammap_i(dtypes):
    output = ["// vi: syntax=c"]
    output += licence_header_cpp()
    output += [
        "",
        "%{",
        '#include "PamMap.hpp"',
        "%}",
        "",
        '%include "DataBlock.i"',
        '%include "std_string.i"',
        '%include "stdint.i"',
        '%include "typedefs.hxx"',
    ]

    output += [
        "",
    ]
    for dtype in dtypes:
        if dtype not in constants.python.underlying_numpy_type:
            continue
        dbtype = to_cpp_blocktype(dtype, full=True)
        output += ["%apply (" + dbtype + " DATAVIEW) {(" + dbtype + " view)}"]

    output += [
        "",
        '%include "PamMap.hpp"',
        "%extend pammap::PamMap {"
    ]
    for dtype in dtypes:
        cpptype = to_cpp_type(dtype, full=True)
        output += [
            "  void update_integer(std::string key, " + cpptype + " value) {",
            "    $self->update(key, std::move(value));",
            "  }",
            "  " + cpptype + " get_" + dtype + "(std::string key) {",
            "    return $self->at<" + cpptype + ">(key);",
            "  }",
            ""
        ]

    for dtype in dtypes:
        if dtype not in constants.python.underlying_numpy_type:
            continue
        dbtype = to_cpp_blocktype(dtype, full=True)
        # Function to update the value in the PamMap by viewing the data
        output += [
            "void update_datablock_" + dtype + "_view(std::string key, " +
            dbtype + " view) {",
            "  $self->update(key, std::move(view));",
            "}",
            ""
        ]

        # Function to update the value in the PamMap by placing a copy to numpy data
        output += [
            "void update_datablock_" + dtype + "_copy(std::string key, " +
            dbtype + " view) {",
            "  // Make a copy of the view including the memory",
            "  " + dbtype + " copy(view, pammap::Memory::OwnCopy);",
            "  $self->update(key, std::move(copy));",
            "}",
            ""
        ]

        # Function to retrieve the value in the PamMap (as a copy)
        output += [
            dbtype + " get_datablock_" + dtype + "_copy(std::string key) {",
            "  return $self->at<" + dbtype + ">(key);"
            "}",
            ""
        ]

    output += ["}"]
    return "\n".join(output)


def main():
    if "-h" in sys.argv or "--help" in sys.argv:
        print("Generate type-specific cxx and hxx files of the project.")
        raise SystemExit()

    with open("typedefs.hxx", "w") as f:
        f.write(generate_typedefs(constants.DTYPES))

    with open("IsSupportedType.hxx", "w") as f:
        f.write(generate_is_supported_type(constants.DTYPES))

    with open("DataBlock.instantiation.hxx", "w") as f:
        f.write(generate_data_block_instantiation(constants.DTYPES))

    with open("PamMap.instantiation.hxx", "w") as f:
        f.write(generate_pammap_instantiation(constants.DTYPES))

    with open("PamMapValue.hxx", "w") as f:
        f.write(generate_pammap_value(constants.DTYPES))

    with open("DataBlock.i", "w") as f:
        f.write(generate_data_block_i(constants.DTYPES))

    with open("PamMap.i", "w") as f:
        f.write(generate_pammap_i(constants.DTYPES))


if __name__ == "__main__":
    main()
