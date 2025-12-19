#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "trie_engine.h"
#include "phrase_boolean_engine.h"

namespace py = pybind11;

PYBIND11_MODULE(advanced_engine, m) {

    py::class_<TrieEngine>(m, "TrieEngine")
        .def(py::init<>())
        .def("insert", &TrieEngine::insert)
        .def("autocomplete", &TrieEngine::autocomplete);

    py::class_<PhraseBooleanEngine>(m, "PhraseBooleanEngine")
        .def(py::init<InvertedIndex&>(), py::keep_alive<1, 2>())
        .def("phrase_search", &PhraseBooleanEngine::phraseSearch)
        .def("boolean_and", &PhraseBooleanEngine::booleanAND)
        .def("boolean_or", &PhraseBooleanEngine::booleanOR)
        .def("boolean_not", &PhraseBooleanEngine::booleanNOT);
}
