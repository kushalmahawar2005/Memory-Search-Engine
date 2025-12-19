# 🔍 In-Memory Search Engine  
### ⚡ C++ Core Engine | 🐍 Python Interface | 🚀 Fast Text Search

---

## 🌟 Project Summary
This project is a **high-performance In-Memory Search Engine** inspired by real-world search systems like **Google Search, IDE search, and file explorers**.

The **core search logic** is implemented in **C++** for speed, while **Python** is used for user interaction and integration using **pybind11**.

> ⚙️ Fast indexing  
> ⚡ Instant search  
> 🧠 Real-world DSA application  

---

## 🧠 How It Works (Simple Idea)

Instead of searching documents again and again, we build an **Inverted Index** once:

---

## 🏗️ Architecture

- **C++** handles indexing and searching (performance-critical)
- **Python** handles user interaction and presentation

---

## ✨ Features
- In-memory document storage
- Inverted index using hash maps
- Single-word and multi-word (AND) search
- Fast lookup using set intersection
- C++ and Python integration using pybind11

---

## 🛠️ Tech Stack
- **Languages:** C++, Python  
- **Binding:** pybind11  
- **DSA Used:** Hash maps, sets, vectors  
- **Build Tool:** g++

---

## ⚙️ Setup & Run

### 1️⃣ Install Dependency
```bash
pip install pybind11

