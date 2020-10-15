`foo` C API Python bindings
***************************

.. toctree::
   :maxdepth: 2
   :caption: Contents:

This library offers `Python 3 <https://www.python.org>`_ bindings for the `foo C API <https://github.com/kmhsonnenkind/foo>`_.

Example
=======

.. code-block:: python

   import foo

   # This will also instantiate a native struct
   instance = foo.Foo()

   # This will wrap to the native library functions
   instance.foob()
   print(f"The foo's baz is {instance.baz}")


`foo` Documentation
====================

.. automodule:: foo
  :members:
