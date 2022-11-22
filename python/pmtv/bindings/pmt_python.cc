/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/* This file is automatically generated using bindtool */

#include <pybind11/complex.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace py = pybind11;

#include <pmtv/pmt.hpp>

// pydoc.h is automatically generated in the build directory
// #include <pmt_pydoc.h>

// PYBIND11_MAKE_OPAQUE(std::map<std::string, pmtv::pmt>)

template <typename T>
static pmtv::pmt _np_to_pmt(py::array_t<T> np_vec)
{
    return pmtv::pmt(
        std::vector<T>(static_cast<const T*>(np_vec.data()),
                       static_cast<const T*>(np_vec.data()) + np_vec.size()));
}

template <typename T>
static py::array_t<T> _pmt_to_np(pmtv::pmt p)
{
    auto vec = get_vector<T>(p);
    return py::array_t<T>(vec.size(), vec.data());
}



template<typename T>
py::object create_numpy_scalar(T val) {
    // usage requires initialized NumPy C-API (call _import_array() before use)
    py::object dt = py::dtype::of<T>();
    PyObject * scal = PyArray_Scalar(&val, (PyArray_Descr*)dt.ptr(), py::int_(sizeof(T)).ptr());
    return py::reinterpret_steal<py::object>(scal);
}

void* pmt_init_numpy()
{
    import_array();
    return NULL;
}

pmtv::pmt buffer_to_pmt(const py::buffer& b) {
    auto buf = b.request();
    if (buf.size == 1) {
    if (buf.format == "f") {
        float val;
        memcpy(&val, buf.ptr, sizeof(val));
        return pmtv::pmt(val);
    } 
    else if (buf.format == "d") {
        double val;
        memcpy(&val, buf.ptr, sizeof(val));
        return pmtv::pmt(val);
    }
    else if (buf.format == "b") {
        uint8_t val;
        memcpy(&val, buf.ptr, sizeof(val));
        return pmtv::pmt(val);
    } 
    else if (buf.format == "H") {
        uint16_t val;
        memcpy(&val, buf.ptr, sizeof(val));
        return pmtv::pmt(val);
    }
    else if (buf.format == "I") {
        uint32_t val;
        memcpy(&val, buf.ptr, sizeof(val));
        return pmtv::pmt(val);
    }
    else if (buf.format == "k") {
        uint64_t val;
        memcpy(&val, buf.ptr, sizeof(val));
        return pmtv::pmt(val);
    } 
    else if (buf.format == "c") {
        int8_t val;
        memcpy(&val, buf.ptr, sizeof(val));
        return pmtv::pmt(val);
    }
    else if (buf.format == "h") {
        int16_t val;
        memcpy(&val, buf.ptr, sizeof(val));
        return pmtv::pmt(val);
    } 
    else if (buf.format == "i") {
        int32_t val;
        memcpy(&val, buf.ptr, sizeof(val));
        return pmtv::pmt(val);
    }
    else if (buf.format == "l") {
        int64_t val;
        memcpy(&val, buf.ptr, sizeof(val));
        return pmtv::pmt(val);
    } 
    }
    std::cerr << buf.format << " " << buf.itemsize << std::endl;

    throw std::runtime_error("Invalid py::buffer object parsing");

}

void bind_pmt(py::module& m)
{
    using pmt = pmtv::pmt;
    pmt_init_numpy();
    py::module_::import("numpy");
    // py::bind_map<std::map<std::string, pmtv::pmt>>(m, "PmtMap");

    py::class_<pmt, std::shared_ptr<pmt>>(m, "pmt")

        // Null Wrapper
        .def(py::init([]() { return pmtv::pmt(); }))

        // // Scalar constructors - python supported types
        // .def(py::init([](const bool& val) { 
        //     return pmtv::pmt(val); }))
        // .def(py::init([](const int64_t& val) { 
        //     return pmtv::pmt(val); }), py::arg{}.noconvert())
        // .def(py::init([](const double& val) { 
        //     return pmtv::pmt(val); }), py::arg{}.noconvert())
        // .def(py::init([](const std::complex<double>& val) { 
        //     return pmtv::pmt(val); }), py::arg{}.noconvert())

        // Vector Wrappers
        // .def(py::init([](const std::vector<bool>& vec) { return pmtf::pmt(vec); }))
        .def(py::init([](const py::array_t<int8_t>& vec) { 
            return _np_to_pmt(vec); 
            }))
        .def(py::init([](const py::array_t<int16_t>& vec) { 
            return _np_to_pmt(vec); }))
        .def(py::init([](const py::array_t<int32_t>& vec) { 
            return _np_to_pmt(vec); }))
        .def(py::init([](const py::array_t<int64_t>& vec) { 
            return _np_to_pmt(vec); }))
        .def(py::init([](const py::array_t<uint8_t>& vec) { 
            return _np_to_pmt(vec); }))
        .def(py::init([](const py::array_t<uint16_t>& vec) { 
            return _np_to_pmt(vec); }))
        .def(py::init([](const py::array_t<uint32_t>& vec) { 
            return _np_to_pmt(vec); }))
        .def(py::init([](const py::array_t<uint64_t>& vec) { 
            return _np_to_pmt(vec); }))
        .def(py::init([](const py::array_t<float>& vec) { 
            return _np_to_pmt(vec); }))
        .def(py::init([](const py::array_t<double>& vec) { 
            return _np_to_pmt(vec); }))
        .def(py::init(
            [](const py::array_t<std::complex<float>>& vec) { 
                return _np_to_pmt(vec); }))
        .def(py::init(
            [](const py::array_t<std::complex<double>>& vec) { 
                return _np_to_pmt(vec); }))
        .def(py::init(
            [](const py::array& vec) { 
                return pmt(); 
                }))
        .def(py::init([](std::vector<pmt>& vec) { 
            // DEBUG: passing in pmt([pmt(1),pmt(2)]) comes in here
            // but each element as a pmt of type std::vector<int8>
            for (auto& a : vec) {
                // int x= pmtv::cast<int>(a);
                
                std::cout << a.index() <<": " << typeid(a).name() << std::endl;
                // std::cout << std::to_string(pmtv::cast<int>(a)) << std::endl;
            }
            return pmt(vec); })
        )

        // The list binding allows pmt([1,2,3]) as well as pmt([pmt(1), pmt(2), pmt(3)])
        // Uniform vectors should use numpy types, anything pmt(list) should become [pmt...]
        // .def(py::init(
        //     [](const py::list& ll) { 
        // //         std::vector<pmt> ret(ll.size());
        // //         size_t idx = 0;
        // //         for (const auto& items : ll) {
        // //             ret[idx++] = pmt(items);
        // //         }
        // //         return pmt(ret);
        // return pmt();
        // }))
        // .def(py::init([](py::array val) { 
        //     return pmtv::pmt(); }), py::arg{}.noconvert())
        // Map constructor
        .def(py::init([](const std::map<std::string, pmtv::pmt>& mm) { return pmtv::pmt(mm); }))

        // Fallback for types not directly mapped to pybind types
        // For supporting e.g. numpy.float32 scalar
        .def(py::init([](py::buffer val) { 
            return buffer_to_pmt(val);
        }))

        .def("__repr__", [](pmtv::pmt obj){
            std::ostringstream os;
            pmtv::operator<<(os, obj);
            return os.str();})

        .def("__str__", [](pmtv::pmt obj){
            std::ostringstream os;
            pmtv::operator<<(os, obj);
            return os.str();})

        .def("__call__",
             [](pmtv::pmt obj) {
                // return std::visit([](const auto& arg) -> pmtv::pmt_nr_var_t { //-> py::object {
                    return std::visit([](const auto& arg) -> py::object {
                    using T = std::decay_t<decltype(arg)>;
                    
                    if constexpr(pmtv::Scalar<T>) { // || pmtv::UniformVector<T> || pmtv::String<T>) {
                        // return pmtv::pmt_nr_var_t(arg);
                        return create_numpy_scalar(arg);
                    }
                    if constexpr(pmtv::UniformVector<T>) { // || pmtv::UniformVector<T> || pmtv::String<T>) {
                        // return pmtv::pmt_nr_var_t(arg);
                        return py::array_t<typename T::value_type>(arg.size(), arg.data());
                    }
                    if constexpr(pmtv::PmtMap<T>) {
                        //     throw new std::runtime_error("Cannot use __call__ operator on PMT Map, use instead get_map method");
                        // put the std::map into a python dict
                        // probably can do this more elegantly by using custom type_caster
                        py::dict d;
                        for (const auto& [k,v] : arg) {
                            d[k.c_str()] = v;
                        }
                        return d;
                    }
                    if constexpr(std::same_as<T, std::vector<pmt>>) { // pmtv::PmtVector<T>) {
                        // throw new py::type_error("Cannot use __call__ operator on PMT Vector, use instead get_vector method");
                        py::list ll;
                        for (const auto& item : arg) {
                            ll.append(item);
                        }
                        return ll;
                        // return py::array_t<typename T::value_type>(arg.size(), arg.data()); //py::array(arg);
                    }
                    throw new py::type_error("__call__ operator only defined for Scalar and UniformVector objects");
                         return py::none();

                    }
                    , obj.get_base());
             })
    ;

    m.def("get_map", &pmtv::get_map<pmt>, "Get a map from a pmt");
    m.def("get_vector", &pmtv::get_vector<pmt,pmt>, "Get a vector from a pmt");
    
    
}
