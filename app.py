import streamlit as st

# -------------------------------------------------
# PAGE CONFIG
# -------------------------------------------------
st.set_page_config(
    page_title="In-Memory Search Engine",
    page_icon="🔍",
    layout="wide"
)

st.title("🔍 In-Memory Search Engine")
st.caption("C++ Backend (pybind11) + Streamlit UI")

st.divider()

# -------------------------------------------------
# LOAD C++ MODULE
# -------------------------------------------------
search_engine = None
module_loaded = False

try:
    import search_engine as cpp_search
    search_engine = cpp_search
    st.success("C++ search_engine module loaded ✅")
    module_loaded = True
except Exception as e:
    try:
        import search_engine_py as py_search
        search_engine = py_search
        st.info("Using Python fallback implementation ⚡")
        module_loaded = True
    except Exception as e2:
        st.error(f"Failed to load both C++ and Python modules: {str(e2)}")
        st.stop()

# -------------------------------------------------
# SESSION STATE (IMPORTANT)
# -------------------------------------------------
if "index" not in st.session_state:
    if search_engine is not None:
        st.session_state.index = search_engine.InvertedIndex()
        st.session_state.trie = search_engine.TrieEngine()
    else:
        st.session_state.index = None
        st.session_state.trie = None

# -------------------------------------------------
# SIDEBAR
# -------------------------------------------------
with st.sidebar:
    st.header("⚙ Search Options")

    mode = st.radio(
        "Choose Mode",
        [
            "Add Document",
            "Ranked Search (TF-IDF)",
            "Phrase Search",
            "Boolean Search",
            "Autocomplete",
            "About Project"
        ]
    )

# =================================================
# ADD DOCUMENT
# =================================================
if mode == "Add Document":
    st.subheader("📄 Add Document")

    doc_id = st.number_input(
        "Document ID",
        min_value=1,
        step=1
    )

    text = st.text_area(
        "Document Text",
        height=150,
        placeholder="Enter document text..."
    )

    if st.button("➕ Add Document", use_container_width=True):
        if text.strip() == "":
            st.warning("Document text cannot be empty")
        elif st.session_state.index is None:
            st.error("Search engine not initialized. Please refresh the page.")
        else:
            st.session_state.index.add_document(doc_id, text)
            st.session_state.trie.insert(text)
            st.success(f"Document {doc_id} added successfully ✅")

# =================================================
# RANKED SEARCH (TF-IDF)
# =================================================
elif mode == "Ranked Search (TF-IDF)":
    st.subheader("🔎 Ranked Search (TF-IDF)")

    query = st.text_input("Search Query")
    top_k = st.slider("Top K Results", 1, 10, 5)

    if st.button("Search", use_container_width=True):
        if query.strip() == "":
            st.warning("Enter a search query")
        else:
            engine = search_engine.RankingEngine(st.session_state.index)
            results = engine.search(query, top_k)

            if not results:
                st.info("No results found")
            else:
                st.markdown("### 📊 Results")
                for doc_id, score in results:
                    st.write(f"📄 **Doc {doc_id}** — Score: `{score:.4f}`")

# =================================================
# PHRASE SEARCH
# =================================================
elif mode == "Phrase Search":
    st.subheader("🧠 Phrase Search")

    phrase = st.text_input("Enter phrase (exact match)")

    if st.button("Search Phrase", use_container_width=True):
        if phrase.strip() == "":
            st.warning("Enter a phrase")
        else:
            engine = search_engine.PhraseBooleanEngine(st.session_state.index)
            results = engine.phrase_search(phrase)

            if not results:
                st.info("No documents found")
            else:
                st.markdown("### 📌 Matching Documents")
                for doc in results:
                    st.write(f"📄 Document ID: {doc}")

# =================================================
# BOOLEAN SEARCH
# =================================================
elif mode == "Boolean Search":
    st.subheader("🔀 Boolean Search")

    st.markdown("Use operators: **AND / OR / NOT**")
    query = st.text_input("Example: apple AND banana NOT mango")

    if st.button("Run Boolean Search", use_container_width=True):
        if query.strip() == "":
            st.warning("Enter a boolean query")
        else:
            engine = search_engine.PhraseBooleanEngine(st.session_state.index)
            results = engine.boolean_search(query.lower())

            if not results:
                st.info("No documents found")
            else:
                st.markdown("### 📌 Matching Documents")
                for doc in results:
                    st.write(f"📄 Document ID: {doc}")

# =================================================
# AUTOCOMPLETE
# =================================================
elif mode == "Autocomplete":
    st.subheader("⚡ Autocomplete (Real-time)")

    prefix = st.text_input(
        "Start typing...",
        placeholder="e.g. mach, lea, data"
    )

    if prefix.strip():
        suggestions = st.session_state.trie.autocomplete(prefix.lower())

        if suggestions:
            st.markdown("### 💡 Suggestions")
            for w in suggestions[:10]:
                st.write(f"🔹 {w}")
        else:
            st.info("No suggestions found")


# =================================================
# ABOUT PROJECT
# =================================================
else:
    st.subheader("📘 About This Project")

    st.markdown(
        """
        ### 🧠 In-Memory Search Engine

        **Core Concepts Used**
        - Inverted Index
        - TF-IDF Ranking
        - Trie for Autocomplete
        - Phrase Search
        - Boolean Query Processing

        **Tech Stack**
        - C++ (High performance backend)
        - pybind11 (Python binding)
        - Streamlit (UI)
        - STL containers

        **Why C++ Backend?**
        - Faster than pure Python
        - Real-world system design
        - Interview-grade project

        ---
        👨‍💻 Built for learning, performance & placements
        """
    )

st.divider()
st.caption("© 2025 • In-Memory Search Engine")
