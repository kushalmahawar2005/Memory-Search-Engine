#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "inverted_index.h"

namespace py = pybind11;

PYBIND11_MODULE(search_engine, m) {
    py::class_<InvertedIndex>(m, "InvertedIndex")
        .def(py::init<>())
        .def("add_document", &InvertedIndex::addDocument)
        .def("contains_word", &InvertedIndex::containsWord)
        .def("get_documents", &InvertedIndex::getDocuments)
        .def("get_positions", &InvertedIndex::getPositions)
        .def("get_filename", &InvertedIndex::getFilename);
}
