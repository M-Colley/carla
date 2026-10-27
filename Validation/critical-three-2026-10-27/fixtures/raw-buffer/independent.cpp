#include "test_raw_data_buffer.cpp"
namespace {
  TEST_F(RawDataBuffer, independent_heap_types_release_after_the_last_export) {
    for (unsigned iteration = 0u; iteration < 100u; ++iteration) {
      auto payload = std::make_shared<Payload>(Payload{3u, 5u, 7u});
      std::weak_ptr<Payload> observer = payload;
      PythonObject type(carla::python::detail::MakeRawDataBufferType());
      ASSERT_NE(type, nullptr);
      PythonObject weak_type(PyWeakref_NewRef(type.get(), nullptr));
      ASSERT_NE(weak_type, nullptr);
      PythonObject view(carla::python::detail::MakeRawDataMemoryView(
          type.get(), reinterpret_cast<char *>(payload->data()), 3, payload));
      ASSERT_NE(view, nullptr);
      type.reset();
      payload.reset();
      PyGC_Collect();
      EXPECT_NE(PyWeakref_GetObject(weak_type.get()), Py_None);
      ASSERT_FALSE(observer.expired());
      view.reset();
      PyGC_Collect();
      EXPECT_TRUE(observer.expired());
      EXPECT_EQ(PyWeakref_GetObject(weak_type.get()), Py_None);
    }
  }
  PyObject *FailAllocation(PyTypeObject *, Py_ssize_t) {
    return PyErr_NoMemory();
  }
  TEST_F(RawDataBuffer, independent_failed_python_allocation_releases_native_holder) {
    auto payload = std::make_shared<Payload>(Payload{3u});
    std::weak_ptr<Payload> observer = payload;
    PythonObject type(carla::python::detail::MakeRawDataBufferType());
    ASSERT_NE(type, nullptr);
    auto *native_type = reinterpret_cast<PyTypeObject *>(type.get());
    const auto allocate = native_type->tp_alloc;
    native_type->tp_alloc = FailAllocation;
    PythonObject view(carla::python::detail::MakeRawDataMemoryView(
        type.get(), reinterpret_cast<char *>(payload->data()), 1, payload));
    native_type->tp_alloc = allocate;
    EXPECT_EQ(view, nullptr);
    EXPECT_TRUE(PyErr_ExceptionMatches(PyExc_MemoryError));
    PyErr_Clear();
    EXPECT_EQ(payload.use_count(), 1);
    payload.reset();
    EXPECT_TRUE(observer.expired());
  }
}
