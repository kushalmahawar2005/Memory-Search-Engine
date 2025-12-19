#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "ranking_engine.h"

namespace py = pybind11;

PYBIND11_MODULE(ranking_engine, m) {
    py::class_<RankingEngine>(m, "RankingEngine")
        .def(py::init<InvertedIndex&, int>(), py::keep_alive<1, 2>())
        .def("keyword_search", &RankingEngine::keywordSearch);
}
