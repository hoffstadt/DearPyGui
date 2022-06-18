========================================
:mod:`typing` --- Support for type hints
========================================

.. module:: typing
   :synopsis: Support for type hints (see :pep:`484`).

.. versionadded:: 3.5

**Source code:** :source:`Lib/typing.py`

.. note::

   The Python runtime does not enforce function and variable type annotations.
   They can be used by third party tools such as type checkers, IDEs, linters,
   etc.

--------------

This module provides runtime support for type hints. The most fundamental
support consists of the types :data:`Any`, :data:`Union`, :data:`Callable`,
:class:`TypeVar`, and :class:`Generic`. For a full specification, please see
:pep:`484`. For a simplified introduction to type hints, see :pep:`483`.


The function below takes and returns a string and is annotated as follows::

   def greeting(name: str) -> str:
       return 'Hello ' + name

In the function ``greeting``, the argument ``name`` is expected to be of type
:class:`str` and the return type :class:`str`. Subtypes are accepted as
arguments.

New features are frequently added to the ``typing`` module.
The `typing_extensions <https://pypi.org/project/typing-extensions/>`_ package
provides backports of these new features to older versions of Python.

.. _relevant-peps:

Relevant PEPs
=============

Since the initial introduction of type hints in :pep:`484` and :pep:`483`, a
number of PEPs have modified and enhanced Python's framework for type
annotations. These include:

* :pep:`526`: Syntax for Variable Annotations
     *Introducing* syntax for annotating variables outside of function
     definitions, and :data:`ClassVar`
* :pep:`544`: Protocols: Structural subtyping (static duck typing)
     *Introducing* :class:`Protocol` and the
     :func:`@runtime_checkable<runtime_checkable>` decorator
* :pep:`585`: Type Hinting Generics In Standard Collections
     *Introducing* :class:`types.GenericAlias` and the ability to use standard
     library classes as :ref:`generic types<types-genericalias>`
* :pep:`586`: Literal Types
     *Introducing* :class:`Literal`
* :pep:`589`: TypedDict: Type Hints for Dictionaries with a Fixed Set of Keys
     *Introducing* :class:`TypedDict`
* :pep:`591`: Adding a final qualifier to typing
     *Introducing* :data:`Final` and the :func:`@final<final>` decorator
* :pep:`593`: Flexible function and variable annotations
     *Introducing* :class:`Annotated`

Type aliases
============

A type alias is defined by assigning the type to the alias. In this example,
``Vector`` and ``list[float]`` will be treated as interchangeable synonyms::

   Vector = list[float]

   def scale(scalar: float, vector: Vector) -> Vector:
       return [scalar * num for num in vector]

   # typechecks; a list of floats qualifies as a Vector.
   new_vector = scale(2.0, [1.0, -4.2, 5.4])

Type aliases are useful for simplifying complex type signatures. For example::

   from collections.abc import Sequence

   ConnectionOptions = dict[str, str]
   Address = tuple[str, int]
   Server = tuple[Address, ConnectionOptions]

   def broadcast_message(message: str, servers: Sequence[Server]) -> None:
       ...

   # The static type checker will treat the previous type signature as
   # being exactly equivalent to this one.
   def broadcast_message(
           message: str,
           servers: Sequence[tuple[tuple[str, int], dict[str, str]]]) -> None:
       ...

Note that ``None`` as a type hint is a special case and is replaced by
``type(None)``.

.. _distinct:

NewType
=======

Use the :func:`NewType` helper function to create distinct types::

   from typing import NewType

   UserId = NewType('UserId', int)
   some_id = UserId(524313)

The static type checker will treat the new type as if it were a subclass
of the original type. This is useful in helping catch logical errors::

   def get_user_name(user_id: UserId) -> str:
       ...

   # typechecks
   user_a = get_user_name(UserId(42351))

   # does not typecheck; an int is not a UserId
   user_b = get_user_name(-1)

You may still perform all ``int`` operations on a variable of type ``UserId``,
but the result will always be of type ``int``. This lets you pass in a
``UserId`` wherever an ``int`` might be expected, but will prevent you from
accidentally creating a ``UserId`` in an invalid way::

   # 'output' is of type 'int', not 'UserId'
   output = UserId(23413) + UserId(54341)

Note that these checks are enforced only by the static type checker. At runtime,
the statement ``Derived = NewType('Derived', Base)`` will make ``Derived`` a
function that immediately returns whatever parameter you pass it. That means
the expression ``Derived(some_value)`` does not create a new class or introduce
any overhead beyond that of a regular function call.

More precisely, the expression ``some_value is Derived(some_value)`` is always
true at runtime.

This also means that it is not possible to create a subtype of ``Derived``
since it is an identity function at runtime, not an actual type::

   from typing import NewType

   UserId = NewType('UserId', int)

   # Fails at runtime and does not typecheck
   class AdminUserId(UserId): pass

However, it is possible to create a :func:`NewType` based on a 'derived' ``NewType``::

   from typing import NewType

   UserId = NewType('UserId', int)

   ProUserId = NewType('ProUserId', UserId)

and typechecking for ``ProUserId`` will work as expected.

See :pep:`484` for more details.

.. note::

   Recall that the use of a type alias declares two types to be *equivalent* to
   one another. Doing ``Alias = Original`` will make the static type checker
   treat ``Alias`` as being *exactly equivalent* to ``Original`` in all cases.
   This is useful when you want to simplify complex type signatures.

   In contrast, ``NewType`` declares one type to be a *subtype* of another.
   Doing ``Derived = NewType('Derived', Original)`` will make the static type
   checker treat ``Derived`` as a *subclass* of ``Original``, which means a
   value of type ``Original`` cannot be used in places where a value of type
   ``Derived`` is expected. This is useful when you want to prevent logic
   errors with minimal runtime cost.

.. versionadded:: 3.5.2

Callable
========

Frameworks expecting callback functions of specific signatures might be
type hinted using ``Callable[[Arg1Type, Arg2Type], ReturnType]``.

For example::

   from collections.abc import Callable

   def feeder(get_next_item: Callable[[], str]) -> None:
       # Body

   def async_query(on_success: Callable[[int], None],
                   on_error: Callable[[int, Exception], None]) -> None:
       # Body

It is possible to declare the return type of a callable without specifying
the call signature by substituting a literal ellipsis
for the list of arguments in the type hint: ``Callable[..., ReturnType]``.

.. _generics:

Generics
========

Since type information about objects kept in containers cannot be statically
inferred in a generic way, abstract base classes have been extended to support
subscription to denote expected types for container elements.

::

   from collections.abc import Mapping, Sequence

   def notify_by_email(employees: Sequence[Employee],
                       overrides: Mapping[str, str]) -> None: ...

Generics can be parameterized by using a factory available in typing
called :class:`TypeVar`.

::

   from collections.abc import Sequence
   from typing import TypeVar

   T = TypeVar('T')      # Declare type variable

   def first(l: Sequence[T]) -> T:   # Generic function
       return l[0]

.. _user-defined-generics:

User-defined generic types
==========================

A user-defined class can be defined as a generic class.

::

   from typing import TypeVar, Generic
   from logging import Logger

   T = TypeVar('T')

   class LoggedVar(Generic[T]):
       def __init__(self, value: T, name: str, logger: Logger) -> None:
           self.name = name
           self.logger = logger
           self.value = value

       def set(self, new: T) -> None:
           self.log('Set ' + repr(self.value))
           self.value = new

       def get(self) -> T:
           self.log('Get ' + repr(self.value))
           return self.value

       def log(self, message: str) -> None:
           self.logger.info('%s: %s', self.name, message)

``Generic[T]`` as a base class defines that the class ``LoggedVar`` takes a
single type parameter ``T`` . This also makes ``T`` valid as a type within the
class body.

The :class:`Generic` base class defines :meth:`~object.__class_getitem__` so
that ``LoggedVar[t]`` is valid as a type::

   from collections.abc import Iterable

   def zero_all_vars(vars: Iterable[LoggedVar[int]]) -> None:
       for var in vars:
           var.set(0)

A generic type can have any number of type variables. All varieties of
:class:`TypeVar` are permissible as parameters for a generic type::

   from typing import TypeVar, Generic, Sequence

   T = TypeVar('T', contravariant=True)
   B = TypeVar('B', bound=Sequence[bytes], covariant=True)
   S = TypeVar('S', int, str)

   class WeirdTrio(Generic[T, B, S]):
       ...

Each type variable argument to :class:`Generic` must be distinct.
This is thus invalid::

   from typing import TypeVar, Generic
   ...

   T = TypeVar('T')

   class Pair(Generic[T, T]):   # INVALID
       ...

You can use multiple inheritance with :class:`Generic`::

   from collections.abc import Sized
   from typing import TypeVar, Generic

   T = TypeVar('T')

   class LinkedList(Sized, Generic[T]):
       ...

When inheriting from generic classes, some type variables could be fixed::

    from collections.abc import Mapping
    from typing import TypeVar

    T = TypeVar('T')

    class MyDict(Mapping[str, T]):
        ...

In this case ``MyDict`` has a single parameter, ``T``.

Using a generic class without specifying type parameters assumes
:data:`Any` for each position. In the following example, ``MyIterable`` is
not generic but implicitly inherits from ``Iterable[Any]``::

   from collections.abc import Iterable

   class MyIterable(Iterable): # Same as Iterable[Any]

User defined generic type aliases are also supported. Examples::

   from collections.abc import Iterable
   from typing import TypeVar, Union
   S = TypeVar('S')
   Response = Union[Iterable[S], int]

   # Return type here is same as Union[Iterable[str], int]
   def response(query: str) -> Response[str]:
       ...

   T = TypeVar('T', int, float, complex)
   Vec = Iterable[tuple[T, T]]

   def inproduct(v: Vec[T]) -> T: # Same as Iterable[tuple[T, T]]
       return sum(x*y for x, y in v)

.. versionchanged:: 3.7
    :class:`Generic` no longer has a custom metaclass.

A user-defined generic class can have ABCs as base classes without a metaclass
conflict. Generic metaclasses are not supported. The outcome of parameterizing
generics is cached, and most types in the typing module are hashable and
comparable for equality.


The :data:`Any` type
====================

A special kind of type is :data:`Any`. A static type checker will treat
every type as being compatible with :data:`Any` and :data:`Any` as being
compatible with every type.

This means that it is possible to perform any operation or method call on a
value of type :data:`Any` and assign it to any variable::

   from typing import Any

   a: Any = None
   a = []          # OK
   a = 2           # OK

   s: str = ''
   s = a           # OK

   def foo(item: Any) -> int:
       # Typechecks; 'item' could be any type,
       # and that type might have a 'bar' method
       item.bar()
       ...

Notice that no typechecking is performed when assigning a value of type
:data:`Any` to a more precise type. For example, the static type checker did
not report an error when assigning ``a`` to ``s`` even though ``s`` was
declared to be of type :class:`str` and receives an :class:`int` value at
runtime!

Furthermore, all functions without a return type or parameter types will
implicitly default to using :data:`Any`::

   def legacy_parser(text):
       ...
       return data

   # A static type checker will treat the above
   # as having the same signature as:
   def legacy_parser(text: Any) -> Any:
       ...
       return data

This behavior allows :data:`Any` to be used as an *escape hatch* when you
need to mix dynamically and statically typed code.

Contrast the behavior of :data:`Any` with the behavior of :class:`object`.
Similar to :data:`Any`, every type is a subtype of :class:`object`. However,
unlike :data:`Any`, the reverse is not true: :class:`object` is *not* a
subtype of every other type.

That means when the type of a value is :class:`object`, a type checker will
reject almost all operations on it, and assigning it to a variable (or using
it as a return value) of a more specialized type is a type error. For example::

   def hash_a(item: object) -> int:
       # Fails; an object does not have a 'magic' method.
       item.magic()
       ...

   def hash_b(item: Any) -> int:
       # Typechecks
       item.magic()
       ...

   # Typechecks, since ints and strs are subclasses of object
   hash_a(42)
   hash_a("foo")

   # Typechecks, since Any is compatible with all types
   hash_b(42)
   hash_b("foo")

Use :class:`object` to indicate that a value could be any type in a typesafe
manner. Use :data:`Any` to indicate that a value is dynamically typed.


Nominal vs structural subtyping
===============================

Initially :pep:`484` defined Python static type system as using
*nominal subtyping*. This means that a class ``A`` is allowed where
a class ``B`` is expected if and only if ``A`` is a subclass of ``B``.

This requirement previously also applied to abstract base classes, such as
:class:`~collections.abc.Iterable`. The problem with this approach is that a class had
to be explicitly marked to support them, which is unpythonic and unlike
what one would normally do in idiomatic dynamically typed Python code.
For example, this conforms to :pep:`484`::

   from collections.abc import Sized, Iterable, Iterator

   class Bucket(Sized, Iterable[int]):
       ...
       def __len__(self) -> int: ...
       def __iter__(self) -> Iterator[int]: ...

:pep:`544` allows to solve this problem by allowing users to write
the above code without explicit base classes in the class definition,
allowing ``Bucket`` to be implicitly considered a subtype of both ``Sized``
and ``Iterable[int]`` by static type checkers. This is known as
*structural subtyping* (or static duck-typing)::

   from collections.abc import Iterator, Iterable

   class Bucket:  # Note: no base classes
       ...
       def __len__(self) -> int: ...
       def __iter__(self) -> Iterator[int]: ...

   def collect(items: Iterable[int]) -> int: ...
   result = collect(Bucket())  # Passes type check

Moreover, by subclassing a special class :class:`Protocol`, a user
can define new custom protocols to fully enjoy structural subtyping
(see examples below).

Module contents
===============

The module defines the following classes, functions and decorators.

.. note::

   This module defines several types that are subclasses of pre-existing
   standard library classes which also extend :class:`Generic`
   to support type variables inside ``[]``.
   These types became redundant in Python 3.9 when the
   corresponding pre-existing classes were enhanced to support ``[]``.

   The redundant types are deprecated as of Python 3.9 but no
   deprecation warnings will be issued by the interpreter.
   It is expected that type checkers will flag the deprecated types
   when the checked program targets Python 3.9 or newer.

   The deprecated types will be removed from the :mod:`typing` module
   in the first Python version released 5 years after the release of Python 3.9.0.
   See details in :pep:`585`—*Type Hinting Generics In Standard Collections*.


Special typing primitives
-------------------------

Special types
"""""""""""""

These can be used as types in annotations and do not support ``[]``.

.. data:: Any

   Special type indicating an unconstrained type.

   * Every type is compatible with :data:`Any`.
   * :data:`Any` is compatible with every type.

.. data:: NoReturn

   Special type indicating that a function never returns.
   For example::

      from typing import NoReturn

      def stop() -> NoReturn:
          raise RuntimeError('no way')

   .. versionadded:: 3.5.4
   .. versionadded:: 3.6.2

Special forms
"""""""""""""

These can be used as types in annotations using ``[]``, each having a unique syntax.

.. data:: Tuple

   Tuple type; ``Tuple[X, Y]`` is the type of a tuple of two items
   with the first item of type X and the second of type Y. The type of
   the empty tuple can be written as ``Tuple[()]``.

   Example: ``Tuple[T1, T2]`` is a tuple of two elements corresponding
   to type variables T1 and T2.  ``Tuple[int, float, str]`` is a tuple
   of an int, a float and a string.

   To specify a variable-length tuple of homogeneous type,
   use literal ellipsis, e.g. ``Tuple[int, ...]``. A plain :data:`Tuple`
   is equivalent to ``Tuple[Any, ...]``, and in turn to :class:`tuple`.

   .. deprecated:: 3.9
      :class:`builtins.tuple <tuple>` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

.. data:: Union

   Union type; ``Union[X, Y]`` means either X or Y.

   To define a union, use e.g. ``Union[int, str]``.  Details:

   * The arguments must be types and there must be at least one.

   * Unions of unions are flattened, e.g.::

       Union[Union[int, str], float] == Union[int, str, float]

   * Unions of a single argument vanish, e.g.::

       Union[int] == int  # The constructor actually returns int

   * Redundant arguments are skipped, e.g.::

       Union[int, str, int] == Union[int, str]

   * When comparing unions, the argument order is ignored, e.g.::

       Union[int, str] == Union[str, int]

   * You cannot subclass or instantiate a union.

   * You cannot write ``Union[X][Y]``.

   * You can use ``Optional[X]`` as a shorthand for ``Union[X, None]``.

   .. versionchanged:: 3.7
      Don't remove explicit subclasses from unions at runtime.

.. data:: Optional

   Optional type.

   ``Optional[X]`` is equivalent to ``Union[X, None]``.

   Note that this is not the same concept as an optional argument,
   which is one that has a default.  An optional argument with a
   default does not require the ``Optional`` qualifier on its type
   annotation just because it is optional. For example::

      def foo(arg: int = 0) -> None:
          ...

   On the other hand, if an explicit value of ``None`` is allowed, the
   use of ``Optional`` is appropriate, whether the argument is optional
   or not. For example::

      def foo(arg: Optional[int] = None) -> None:
          ...

.. data:: Callable

   Callable type; ``Callable[[int], str]`` is a function of (int) -> str.

   The subscription syntax must always be used with exactly two
   values: the argument list and the return type.  The argument list
   must be a list of types or an ellipsis; the return type must be
   a single type.

   There is no syntax to indicate optional or keyword arguments;
   such function types are rarely used as callback types.
   ``Callable[..., ReturnType]`` (literal ellipsis) can be used to
   type hint a callable taking any number of arguments and returning
   ``ReturnType``.  A plain :data:`Callable` is equivalent to
   ``Callable[..., Any]``, and in turn to
   :class:`collections.abc.Callable`.

   .. deprecated:: 3.9
      :class:`collections.abc.Callable` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

.. class:: Type(Generic[CT_co])

   A variable annotated with ``C`` may accept a value of type ``C``. In
   contrast, a variable annotated with ``Type[C]`` may accept values that are
   classes themselves -- specifically, it will accept the *class object* of
   ``C``. For example::

      a = 3         # Has type 'int'
      b = int       # Has type 'Type[int]'
      c = type(a)   # Also has type 'Type[int]'

   Note that ``Type[C]`` is covariant::

      class User: ...
      class BasicUser(User): ...
      class ProUser(User): ...
      class TeamUser(User): ...

      # Accepts User, BasicUser, ProUser, TeamUser, ...
      def make_new_user(user_class: Type[User]) -> User:
          # ...
          return user_class()

   The fact that ``Type[C]`` is covariant implies that all subclasses of
   ``C`` should implement the same constructor signature and class method
   signatures as ``C``. The type checker should flag violations of this,
   but should also allow constructor calls in subclasses that match the
   constructor calls in the indicated base class. How the type checker is
   required to handle this particular case may change in future revisions of
   :pep:`484`.

   The only legal parameters for :class:`Type` are classes, :data:`Any`,
   :ref:`type variables <generics>`, and unions of any of these types.
   For example::

      def new_non_team_user(user_class: Type[Union[BasicUser, ProUser]]): ...

   ``Type[Any]`` is equivalent to ``Type`` which in turn is equivalent
   to ``type``, which is the root of Python's metaclass hierarchy.

   .. versionadded:: 3.5.2

   .. deprecated:: 3.9
      :class:`builtins.type <type>` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

.. data:: Literal

   A type that can be used to indicate to type checkers that the
   corresponding variable or function parameter has a value equivalent to
   the provided literal (or one of several literals). For example::

      def validate_simple(data: Any) -> Literal[True]:  # always returns True
          ...

      MODE = Literal['r', 'rb', 'w', 'wb']
      def open_helper(file: str, mode: MODE) -> str:
          ...

      open_helper('/some/path', 'r')  # Passes type check
      open_helper('/other/path', 'typo')  # Error in type checker

   ``Literal[...]`` cannot be subclassed. At runtime, an arbitrary value
   is allowed as type argument to ``Literal[...]``, but type checkers may
   impose restrictions. See :pep:`586` for more details about literal types.

   .. versionadded:: 3.8

   .. versionchanged:: 3.9.1
      ``Literal`` now de-duplicates parameters.  Equality comparisons of
      ``Literal`` objects are no longer order dependent. ``Literal`` objects
      will now raise a :exc:`TypeError` exception during equality comparisons
      if one of their parameters are not :term:`hashable`.

.. data:: ClassVar

   Special type construct to mark class variables.

   As introduced in :pep:`526`, a variable annotation wrapped in ClassVar
   indicates that a given attribute is intended to be used as a class variable
   and should not be set on instances of that class. Usage::

      class Starship:
          stats: ClassVar[dict[str, int]] = {} # class variable
          damage: int = 10                     # instance variable

   :data:`ClassVar` accepts only types and cannot be further subscribed.

   :data:`ClassVar` is not a class itself, and should not
   be used with :func:`isinstance` or :func:`issubclass`.
   :data:`ClassVar` does not change Python runtime behavior, but
   it can be used by third-party type checkers. For example, a type checker
   might flag the following code as an error::

      enterprise_d = Starship(3000)
      enterprise_d.stats = {} # Error, setting class variable on instance
      Starship.stats = {}     # This is OK

   .. versionadded:: 3.5.3

.. data:: Final

   A special typing construct to indicate to type checkers that a name
   cannot be re-assigned or overridden in a subclass. For example::

      MAX_SIZE: Final = 9000
      MAX_SIZE += 1  # Error reported by type checker

      class Connection:
          TIMEOUT: Final[int] = 10

      class FastConnector(Connection):
          TIMEOUT = 1  # Error reported by type checker

   There is no runtime checking of these properties. See :pep:`591` for
   more details.

   .. versionadded:: 3.8

.. data:: Annotated

   A type, introduced in :pep:`593` (``Flexible function and variable
   annotations``), to decorate existing types with context-specific metadata
   (possibly multiple pieces of it, as ``Annotated`` is variadic).
   Specifically, a type ``T`` can be annotated with metadata ``x`` via the
   typehint ``Annotated[T, x]``. This metadata can be used for either static
   analysis or at runtime. If a library (or tool) encounters a typehint
   ``Annotated[T, x]`` and has no special logic for metadata ``x``, it
   should ignore it and simply treat the type as ``T``. Unlike the
   ``no_type_check`` functionality that currently exists in the ``typing``
   module which completely disables typechecking annotations on a function
   or a class, the ``Annotated`` type allows for both static typechecking
   of ``T`` (e.g., via mypy or Pyre, which can safely ignore ``x``)
   together with runtime access to ``x`` within a specific application.

   Ultimately, the responsibility of how to interpret the annotations (if
   at all) is the responsibility of the tool or library encountering the
   ``Annotated`` type. A tool or library encountering an ``Annotated`` type
   can scan through the annotations to determine if they are of interest
   (e.g., using ``isinstance()``).

   When a tool or a library does not support annotations or encounters an
   unknown annotation it should just ignore it and treat annotated type as
   the underlying type.

   It's up to the tool consuming the annotations to decide whether the
   client is allowed to have several annotations on one type and how to
   merge those annotations.

   Since the ``Annotated`` type allows you to put several annotations of
   the same (or different) type(s) on any node, the tools or libraries
   consuming those annotations are in charge of dealing with potential
   duplicates. For example, if you are doing value range analysis you might
   allow this::

       T1 = Annotated[int, ValueRange(-10, 5)]
       T2 = Annotated[T1, ValueRange(-20, 3)]

   Passing ``include_extras=True`` to :func:`get_type_hints` lets one
   access the extra annotations at runtime.

   The details of the syntax:

   * The first argument to ``Annotated`` must be a valid type

   * Multiple type annotations are supported (``Annotated`` supports variadic
     arguments)::

       Annotated[int, ValueRange(3, 10), ctype("char")]

   * ``Annotated`` must be called with at least two arguments (
     ``Annotated[int]`` is not valid)

   * The order of the annotations is preserved and matters for equality
     checks::

       Annotated[int, ValueRange(3, 10), ctype("char")] != Annotated[
           int, ctype("char"), ValueRange(3, 10)
       ]

   * Nested ``Annotated`` types are flattened, with metadata ordered
     starting with the innermost annotation::

       Annotated[Annotated[int, ValueRange(3, 10)], ctype("char")] == Annotated[
           int, ValueRange(3, 10), ctype("char")
       ]

   * Duplicated annotations are not removed::

       Annotated[int, ValueRange(3, 10)] != Annotated[
           int, ValueRange(3, 10), ValueRange(3, 10)
       ]

   * ``Annotated`` can be used with nested and generic aliases::

       T = TypeVar('T')
       Vec = Annotated[list[tuple[T, T]], MaxLen(10)]
       V = Vec[int]

       V == Annotated[list[tuple[int, int]], MaxLen(10)]

   .. versionadded:: 3.9

Building generic types
""""""""""""""""""""""

These are not used in annotations. They are building blocks for creating generic types.

.. class:: Generic

   Abstract base class for generic types.

   A generic type is typically declared by inheriting from an
   instantiation of this class with one or more type variables.
   For example, a generic mapping type might be defined as::

      class Mapping(Generic[KT, VT]):
          def __getitem__(self, key: KT) -> VT:
              ...
              # Etc.

   This class can then be used as follows::

      X = TypeVar('X')
      Y = TypeVar('Y')

      def lookup_name(mapping: Mapping[X, Y], key: X, default: Y) -> Y:
          try:
              return mapping[key]
          except KeyError:
              return default

.. class:: TypeVar

    Type variable.

    Usage::

      T = TypeVar('T')  # Can be anything
      S = TypeVar('S', bound=str)  # Can be any subtype of str
      A = TypeVar('A', str, bytes)  # Must be exactly str or bytes

    Type variables exist primarily for the benefit of static type
    checkers.  They serve as the parameters for generic types as well
    as for generic function definitions.  See :class:`Generic` for more
    information on generic types.  Generic functions work as follows::

       def repeat(x: T, n: int) -> Sequence[T]:
           """Return a list containing n references to x."""
           return [x]*n


       def print_capitalized(x: S) -> S:
           """Print x capitalized, and return x."""
           print(x.capitalize())
           return x


       def concatenate(x: A, y: A) -> A:
           """Add two strings or bytes objects together."""
           return x + y

    Note that type variables can be *bound*, *constrained*, or neither, but
    cannot be both bound *and* constrained.

    Constrained type variables and bound type variables have different
    semantics in several important ways. Using a *constrained* type variable
    means that the ``TypeVar`` can only ever be solved as being exactly one of
    the constraints given::

       a = concatenate('one', 'two')  # Ok, variable 'a' has type 'str'
       b = concatenate(StringSubclass('one'), StringSubclass('two'))  # Inferred type of variable 'b' is 'str',
                                                                      # despite 'StringSubclass' being passed in
       c = concatenate('one', b'two')  # error: type variable 'A' can be either 'str' or 'bytes' in a function call, but not both

    Using a *bound* type variable, however, means that the ``TypeVar`` will be
    solved using the most specific type possible::

       print_capitalized('a string')  # Ok, output has type 'str'

       class StringSubclass(str):
           pass

       print_capitalized(StringSubclass('another string'))  # Ok, output has type 'StringSubclass'
       print_capitalized(45)  # error: int is not a subtype of str

    Type variables can be bound to concrete types, abstract types (ABCs or
    protocols), and even unions of types::

       U = TypeVar('U', bound=str|bytes)  # Can be any subtype of the union str|bytes
       V = TypeVar('V', bound=SupportsAbs)  # Can be anything with an __abs__ method

    Bound type variables are particularly useful for annotating
    :func:`classmethods <classmethod>` that serve as alternative constructors.
    In the following example (©
    `Raymond Hettinger <https://www.youtube.com/watch?v=HTLu2DFOdTg>`_), the
    type variable ``C`` is bound to the ``Circle`` class through the use of a
    forward reference. Using this type variable to annotate the
    ``with_circumference`` classmethod, rather than hardcoding the return type
    as ``Circle``, means that a type checker can correctly infer the return
    type even if the method is called on a subclass::

       import math

       C = TypeVar('C', bound='Circle')

       class Circle:
           """An abstract circle"""

           def __init__(self, radius: float) -> None:
               self.radius = radius

           # Use a type variable to show that the return type
           # will always be an instance of whatever `cls` is
           @classmethod
           def with_circumference(cls: type[C], circumference: float) -> C:
               """Create a circle with the specified circumference"""
               radius = circumference / (math.pi * 2)
               return cls(radius)


       class Tire(Circle):
           """A specialised circle (made out of rubber)"""

           MATERIAL = 'rubber'


       c = Circle.with_circumference(3)  # Ok, variable 'c' has type 'Circle'
       t = Tire.with_circumference(4)  # Ok, variable 't' has type 'Tire' (not 'Circle')

    At runtime, ``isinstance(x, T)`` will raise :exc:`TypeError`.  In general,
    :func:`isinstance` and :func:`issubclass` should not be used with types.

    Type variables may be marked covariant or contravariant by passing
    ``covariant=True`` or ``contravariant=True``.  See :pep:`484` for more
    details.  By default, type variables are invariant.

.. data:: AnyStr

   ``AnyStr`` is a :class:`constrained type variable <TypeVar>` defined as
   ``AnyStr = TypeVar('AnyStr', str, bytes)``.

   It is meant to be used for functions that may accept any kind of string
   without allowing different kinds of strings to mix. For example::

      def concat(a: AnyStr, b: AnyStr) -> AnyStr:
          return a + b

      concat(u"foo", u"bar")  # Ok, output has type 'unicode'
      concat(b"foo", b"bar")  # Ok, output has type 'bytes'
      concat(u"foo", b"bar")  # Error, cannot mix unicode and bytes

.. class:: Protocol(Generic)

   Base class for protocol classes. Protocol classes are defined like this::

      class Proto(Protocol):
          def meth(self) -> int:
              ...

   Such classes are primarily used with static type checkers that recognize
   structural subtyping (static duck-typing), for example::

      class C:
          def meth(self) -> int:
              return 0

      def func(x: Proto) -> int:
          return x.meth()

      func(C())  # Passes static type check

   See :pep:`544` for details. Protocol classes decorated with
   :func:`runtime_checkable` (described later) act as simple-minded runtime
   protocols that check only the presence of given attributes, ignoring their
   type signatures.

   Protocol classes can be generic, for example::

      class GenProto(Protocol[T]):
          def meth(self) -> T:
              ...

   .. versionadded:: 3.8

.. decorator:: runtime_checkable

   Mark a protocol class as a runtime protocol.

   Such a protocol can be used with :func:`isinstance` and :func:`issubclass`.
   This raises :exc:`TypeError` when applied to a non-protocol class.  This
   allows a simple-minded structural check, very similar to "one trick ponies"
   in :mod:`collections.abc` such as :class:`~collections.abc.Iterable`.  For example::

      @runtime_checkable
      class Closable(Protocol):
          def close(self): ...

      assert isinstance(open('/some/file'), Closable)

   .. note::

        :func:`runtime_checkable` will check only the presence of the required methods,
        not their type signatures! For example, :class:`builtins.complex <complex>`
        implements :func:`__float__`, therefore it passes an :func:`issubclass` check
        against :class:`SupportsFloat`. However, the ``complex.__float__`` method
        exists only to raise a :class:`TypeError` with a more informative message.

   .. versionadded:: 3.8

Other special directives
""""""""""""""""""""""""

These are not used in annotations. They are building blocks for declaring types.

.. class:: NamedTuple

   Typed version of :func:`collections.namedtuple`.

   Usage::

       class Employee(NamedTuple):
           name: str
           id: int

   This is equivalent to::

       Employee = collections.namedtuple('Employee', ['name', 'id'])

   To give a field a default value, you can assign to it in the class body::

      class Employee(NamedTuple):
          name: str
          id: int = 3

      employee = Employee('Guido')
      assert employee.id == 3

   Fields with a default value must come after any fields without a default.

   The resulting class has an extra attribute ``__annotations__`` giving a
   dict that maps the field names to the field types.  (The field names are in
   the ``_fields`` attribute and the default values are in the
   ``_field_defaults`` attribute both of which are part of the namedtuple
   API.)

   ``NamedTuple`` subclasses can also have docstrings and methods::

      class Employee(NamedTuple):
          """Represents an employee."""
          name: str
          id: int = 3

          def __repr__(self) -> str:
              return f'<Employee {self.name}, id={self.id}>'

   Backward-compatible usage::

       Employee = NamedTuple('Employee', [('name', str), ('id', int)])

   .. versionchanged:: 3.6
      Added support for :pep:`526` variable annotation syntax.

   .. versionchanged:: 3.6.1
      Added support for default values, methods, and docstrings.

   .. versionchanged:: 3.8
      The ``_field_types`` and ``__annotations__`` attributes are
      now regular dictionaries instead of instances of ``OrderedDict``.

   .. versionchanged:: 3.9
      Removed the ``_field_types`` attribute in favor of the more
      standard ``__annotations__`` attribute which has the same information.

.. function:: NewType(name, tp)

   A helper function to indicate a distinct type to a typechecker,
   see :ref:`distinct`. At runtime it returns a function that returns
   its argument. Usage::

      UserId = NewType('UserId', int)
      first_user = UserId(1)

   .. versionadded:: 3.5.2

.. class:: TypedDict(dict)

   Special construct to add type hints to a dictionary.
   At runtime it is a plain :class:`dict`.

   ``TypedDict`` declares a dictionary type that expects all of its
   instances to have a certain set of keys, where each key is
   associated with a value of a consistent type. This expectation
   is not checked at runtime but is only enforced by type checkers.
   Usage::

      class Point2D(TypedDict):
          x: int
          y: int
          label: str

      a: Point2D = {'x': 1, 'y': 2, 'label': 'good'}  # OK
      b: Point2D = {'z': 3, 'label': 'bad'}           # Fails type check

      assert Point2D(x=1, y=2, label='first') == dict(x=1, y=2, label='first')

   To allow using this feature with older versions of Python that do not
   support :pep:`526`, ``TypedDict`` supports two additional equivalent
   syntactic forms:

   * Using a literal :class:`dict` as the second argument::

      Point2D = TypedDict('Point2D', {'x': int, 'y': int, 'label': str})

   * Using keyword arguments::

      Point2D = TypedDict('Point2D', x=int, y=int, label=str)

   The functional syntax should also be used when any of the keys are not valid
   :ref:`identifiers`, for example because they are keywords or contain hyphens.
   Example::

      # raises SyntaxError
      class Point2D(TypedDict):
          in: int  # 'in' is a keyword
          x-y: int  # name with hyphens

      # OK, functional syntax
      Point2D = TypedDict('Point2D', {'in': int, 'x-y': int})

   By default, all keys must be present in a ``TypedDict``. It is possible to
   override this by specifying totality.
   Usage::

      class Point2D(TypedDict, total=False):
          x: int
          y: int

      # Alternative syntax
      Point2D = TypedDict('Point2D', {'x': int, 'y': int}, total=False)

   This means that a ``Point2D`` ``TypedDict`` can have any of the keys
   omitted. A type checker is only expected to support a literal ``False`` or
   ``True`` as the value of the ``total`` argument. ``True`` is the default,
   and makes all items defined in the class body required.

   It is possible for a ``TypedDict`` type to inherit from one or more other ``TypedDict`` types
   using the class-based syntax.
   Usage::

      class Point3D(Point2D):
          z: int

   ``Point3D`` has three items: ``x``, ``y`` and ``z``. It is equivalent to this
   definition::

      class Point3D(TypedDict):
          x: int
          y: int
          z: int

   A ``TypedDict`` cannot inherit from a non-TypedDict class,
   notably including :class:`Generic`. For example::

      class X(TypedDict):
          x: int

      class Y(TypedDict):
          y: int

      class Z(object): pass  # A non-TypedDict class

      class XY(X, Y): pass  # OK

      class XZ(X, Z): pass  # raises TypeError

      T = TypeVar('T')
      class XT(X, Generic[T]): pass  # raises TypeError

   A ``TypedDict`` can be introspected via :attr:`__annotations__`,
   :attr:`__total__`, :attr:`__required_keys__`, and :attr:`__optional_keys__`.

   .. attribute:: __total__

      ``Point2D.__total__`` gives the value of the ``total`` argument.
      Example::

         >>> from typing import TypedDict
         >>> class Point2D(TypedDict): pass
         >>> Point2D.__total__
         True
         >>> class Point2D(TypedDict, total=False): pass
         >>> Point2D.__total__
         False
         >>> class Point3D(Point2D): pass
         >>> Point3D.__total__
         True

   .. attribute:: __required_keys__
   .. attribute:: __optional_keys__

      ``Point2D.__required_keys__`` and ``Point2D.__optional_keys__`` return
      :class:`frozenset` objects containing required and non-required keys, respectively.
      Currently the only way to declare both required and non-required keys in the
      same ``TypedDict`` is mixed inheritance, declaring a ``TypedDict`` with one value
      for the ``total`` argument and then inheriting it from another ``TypedDict`` with
      a different value for ``total``.
      Usage::

         >>> class Point2D(TypedDict, total=False):
         ...     x: int
         ...     y: int
         ...
         >>> class Point3D(Point2D):
         ...     z: int
         ...
         >>> Point3D.__required_keys__ == frozenset({'z'})
         True
         >>> Point3D.__optional_keys__ == frozenset({'x', 'y'})
         True

   See :pep:`589` for more examples and detailed rules of using ``TypedDict``.

   .. versionadded:: 3.8

Generic concrete collections
----------------------------

Corresponding to built-in types
"""""""""""""""""""""""""""""""

.. class:: Dict(dict, MutableMapping[KT, VT])

   A generic version of :class:`dict`.
   Useful for annotating return types. To annotate arguments it is preferred
   to use an abstract collection type such as :class:`Mapping`.

   This type can be used as follows::

      def count_words(text: str) -> Dict[str, int]:
          ...

   .. deprecated:: 3.9
      :class:`builtins.dict <dict>` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

.. class:: List(list, MutableSequence[T])

   Generic version of :class:`list`.
   Useful for annotating return types. To annotate arguments it is preferred
   to use an abstract collection type such as :class:`Sequence` or
   :class:`Iterable`.

   This type may be used as follows::

      T = TypeVar('T', int, float)

      def vec2(x: T, y: T) -> List[T]:
          return [x, y]

      def keep_positives(vector: Sequence[T]) -> List[T]:
          return [item for item in vector if item > 0]

   .. deprecated:: 3.9
      :class:`builtins.list <list>` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

.. class:: Set(set, MutableSet[T])

   A generic version of :class:`builtins.set <set>`.
   Useful for annotating return types. To annotate arguments it is preferred
   to use an abstract collection type such as :class:`AbstractSet`.

   .. deprecated:: 3.9
      :class:`builtins.set <set>` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

.. class:: FrozenSet(frozenset, AbstractSet[T_co])

   A generic version of :class:`builtins.frozenset <frozenset>`.

   .. deprecated:: 3.9
      :class:`builtins.frozenset <frozenset>` now supports ``[]``. See
      :pep:`585` and :ref:`types-genericalias`.

.. note:: :data:`Tuple` is a special form.

Corresponding to types in :mod:`collections`
""""""""""""""""""""""""""""""""""""""""""""

.. class:: DefaultDict(collections.defaultdict, MutableMapping[KT, VT])

   A generic version of :class:`collections.defaultdict`.

   .. versionadded:: 3.5.2

   .. deprecated:: 3.9
      :class:`collections.defaultdict` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

.. class:: OrderedDict(collections.OrderedDict, MutableMapping[KT, VT])

   A generic version of :class:`collections.OrderedDict`.

   .. versionadded:: 3.7.2

   .. deprecated:: 3.9
      :class:`collections.OrderedDict` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

.. class:: ChainMap(collections.ChainMap, MutableMapping[KT, VT])

   A generic version of :class:`collections.ChainMap`.

   .. versionadded:: 3.5.4
   .. versionadded:: 3.6.1

   .. deprecated:: 3.9
      :class:`collections.ChainMap` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

.. class:: Counter(collections.Counter, Dict[T, int])

   A generic version of :class:`collections.Counter`.

   .. versionadded:: 3.5.4
   .. versionadded:: 3.6.1

   .. deprecated:: 3.9
      :class:`collections.Counter` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

.. class:: Deque(deque, MutableSequence[T])

   A generic version of :class:`collections.deque`.

   .. versionadded:: 3.5.4
   .. versionadded:: 3.6.1

   .. deprecated:: 3.9
      :class:`collections.deque` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

Other concrete types
""""""""""""""""""""

.. class:: IO
           TextIO
           BinaryIO

   Generic type ``IO[AnyStr]`` and its subclasses ``TextIO(IO[str])``
   and ``BinaryIO(IO[bytes])``
   represent the types of I/O streams such as returned by
   :func:`open`.

   .. deprecated-removed:: 3.8 3.12
      These types are also in the ``typing.io`` namespace, which was
      never supported by type checkers and will be removed.

.. class:: Pattern
           Match

   These type aliases
   correspond to the return types from :func:`re.compile` and
   :func:`re.match`.  These types (and the corresponding functions)
   are generic in ``AnyStr`` and can be made specific by writing
   ``Pattern[str]``, ``Pattern[bytes]``, ``Match[str]``, or
   ``Match[bytes]``.

   .. deprecated-removed:: 3.8 3.12
      These types are also in the ``typing.re`` namespace, which was
      never supported by type checkers and will be removed.

   .. deprecated:: 3.9
      Classes ``Pattern`` and ``Match`` from :mod:`re` now support ``[]``.
      See :pep:`585` and :ref:`types-genericalias`.

.. class:: Text

   ``Text`` is an alias for ``str``. It is provided to supply a forward
   compatible path for Python 2 code: in Python 2, ``Text`` is an alias for
   ``unicode``.

   Use ``Text`` to indicate that a value must contain a unicode string in
   a manner that is compatible with both Python 2 and Python 3::

       def add_unicode_checkmark(text: Text) -> Text:
           return text + u' \u2713'

   .. versionadded:: 3.5.2

Abstract Base Classes
---------------------

Corresponding to collections in :mod:`collections.abc`
""""""""""""""""""""""""""""""""""""""""""""""""""""""

.. class:: AbstractSet(Sized, Collection[T_co])

   A generic version of :class:`collections.abc.Set`.

   .. deprecated:: 3.9
      :class:`collections.abc.Set` now supports ``[]``. See :pep:`585` and
      :ref:`types-genericalias`.

.. class:: ByteString(Sequence[int])

   A generic version of :class:`collections.abc.ByteString`.

   This type represents the types :class:`bytes`, :class:`bytearray`,
   and :class:`memoryview` of byte sequences.

   As a shorthand for this type, :class:`bytes` can be used to
   annotate arguments of any of the types mentioned above.

   .. deprecated:: 3.9
      :class:`collections.abc.ByteString` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: Collection(Sized, Iterable[T_co], Container[T_co])

   A generic version of :class:`collections.abc.Collection`

   .. versionadded:: 3.6.0

   .. deprecated:: 3.9
      :class:`collections.abc.Collection` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: Container(Generic[T_co])

   A generic version of :class:`collections.abc.Container`.

   .. deprecated:: 3.9
      :class:`collections.abc.Container` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: ItemsView(MappingView, Generic[KT_co, VT_co])

   A generic version of :class:`collections.abc.ItemsView`.

   .. deprecated:: 3.9
      :class:`collections.abc.ItemsView` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: KeysView(MappingView[KT_co], AbstractSet[KT_co])

   A generic version of :class:`collections.abc.KeysView`.

   .. deprecated:: 3.9
      :class:`collections.abc.KeysView` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: Mapping(Sized, Collection[KT], Generic[VT_co])

   A generic version of :class:`collections.abc.Mapping`.
   This type can be used as follows::

     def get_position_in_index(word_list: Mapping[str, int], word: str) -> int:
         return word_list[word]

   .. deprecated:: 3.9
      :class:`collections.abc.Mapping` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: MappingView(Sized, Iterable[T_co])

   A generic version of :class:`collections.abc.MappingView`.

   .. deprecated:: 3.9
      :class:`collections.abc.MappingView` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: MutableMapping(Mapping[KT, VT])

   A generic version of :class:`collections.abc.MutableMapping`.

   .. deprecated:: 3.9
      :class:`collections.abc.MutableMapping` now supports ``[]``. See
      :pep:`585` and :ref:`types-genericalias`.

.. class:: MutableSequence(Sequence[T])

   A generic version of :class:`collections.abc.MutableSequence`.

   .. deprecated:: 3.9
      :class:`collections.abc.MutableSequence` now supports ``[]``. See
      :pep:`585` and :ref:`types-genericalias`.

.. class:: MutableSet(AbstractSet[T])

   A generic version of :class:`collections.abc.MutableSet`.

   .. deprecated:: 3.9
      :class:`collections.abc.MutableSet` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: Sequence(Reversible[T_co], Collection[T_co])

   A generic version of :class:`collections.abc.Sequence`.

   .. deprecated:: 3.9
      :class:`collections.abc.Sequence` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: ValuesView(MappingView[VT_co])

   A generic version of :class:`collections.abc.ValuesView`.

   .. deprecated:: 3.9
      :class:`collections.abc.ValuesView` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

Corresponding to other types in :mod:`collections.abc`
""""""""""""""""""""""""""""""""""""""""""""""""""""""

.. class:: Iterable(Generic[T_co])

   A generic version of :class:`collections.abc.Iterable`.

   .. deprecated:: 3.9
      :class:`collections.abc.Iterable` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: Iterator(Iterable[T_co])

   A generic version of :class:`collections.abc.Iterator`.

   .. deprecated:: 3.9
      :class:`collections.abc.Iterator` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: Generator(Iterator[T_co], Generic[T_co, T_contra, V_co])

   A generator can be annotated by the generic type
   ``Generator[YieldType, SendType, ReturnType]``. For example::

      def echo_round() -> Generator[int, float, str]:
          sent = yield 0
          while sent >= 0:
              sent = yield round(sent)
          return 'Done'

   Note that unlike many other generics in the typing module, the ``SendType``
   of :class:`Generator` behaves contravariantly, not covariantly or
   invariantly.

   If your generator will only yield values, set the ``SendType`` and
   ``ReturnType`` to ``None``::

      def infinite_stream(start: int) -> Generator[int, None, None]:
          while True:
              yield start
              start += 1

   Alternatively, annotate your generator as having a return type of
   either ``Iterable[YieldType]`` or ``Iterator[YieldType]``::

      def infinite_stream(start: int) -> Iterator[int]:
          while True:
              yield start
              start += 1

   .. deprecated:: 3.9
      :class:`collections.abc.Generator` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: Hashable

   An alias to :class:`collections.abc.Hashable`

.. class:: Reversible(Iterable[T_co])

   A generic version of :class:`collections.abc.Reversible`.

   .. deprecated:: 3.9
      :class:`collections.abc.Reversible` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: Sized

   An alias to :class:`collections.abc.Sized`

Asynchronous programming
""""""""""""""""""""""""

.. class:: Coroutine(Awaitable[V_co], Generic[T_co, T_contra, V_co])

   A generic version of :class:`collections.abc.Coroutine`.
   The variance and order of type variables
   correspond to those of :class:`Generator`, for example::

      from collections.abc import Coroutine
      c: Coroutine[list[str], str, int]  # Some coroutine defined elsewhere
      x = c.send('hi')                   # Inferred type of 'x' is list[str]
      async def bar() -> None:
          y = await c                    # Inferred type of 'y' is int

   .. versionadded:: 3.5.3

   .. deprecated:: 3.9
      :class:`collections.abc.Coroutine` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: AsyncGenerator(AsyncIterator[T_co], Generic[T_co, T_contra])

   An async generator can be annotated by the generic type
   ``AsyncGenerator[YieldType, SendType]``. For example::

      async def echo_round() -> AsyncGenerator[int, float]:
          sent = yield 0
          while sent >= 0.0:
              rounded = await round(sent)
              sent = yield rounded

   Unlike normal generators, async generators cannot return a value, so there
   is no ``ReturnType`` type parameter. As with :class:`Generator`, the
   ``SendType`` behaves contravariantly.

   If your generator will only yield values, set the ``SendType`` to
   ``None``::

      async def infinite_stream(start: int) -> AsyncGenerator[int, None]:
          while True:
              yield start
              start = await increment(start)

   Alternatively, annotate your generator as having a return type of
   either ``AsyncIterable[YieldType]`` or ``AsyncIterator[YieldType]``::

      async def infinite_stream(start: int) -> AsyncIterator[int]:
          while True:
              yield start
              start = await increment(start)

   .. versionadded:: 3.6.1

   .. deprecated:: 3.9
      :class:`collections.abc.AsyncGenerator` now supports ``[]``. See
      :pep:`585` and :ref:`types-genericalias`.

.. class:: AsyncIterable(Generic[T_co])

   A generic version of :class:`collections.abc.AsyncIterable`.

   .. versionadded:: 3.5.2

   .. deprecated:: 3.9
      :class:`collections.abc.AsyncIterable` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: AsyncIterator(AsyncIterable[T_co])

   A generic version of :class:`collections.abc.AsyncIterator`.

   .. versionadded:: 3.5.2

   .. deprecated:: 3.9
      :class:`collections.abc.AsyncIterator` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.

.. class:: Awaitable(Generic[T_co])

   A generic version of :class:`collections.abc.Awaitable`.

   .. versionadded:: 3.5.2

   .. deprecated:: 3.9
      :class:`collections.abc.Awaitable` now supports ``[]``. See :pep:`585`
      and :ref:`types-genericalias`.


Context manager types
"""""""""""""""""""""

.. class:: ContextManager(Generic[T_co])

   A generic version of :class:`contextlib.AbstractContextManager`.

   .. versionadded:: 3.5.4
   .. versionadded:: 3.6.0

   .. deprecated:: 3.9
      :class:`contextlib.AbstractContextManager` now supports ``[]``. See
      :pep:`585` and :ref:`types-genericalias`.

.. class:: AsyncContextManager(Generic[T_co])

   A generic version of :class:`contextlib.AbstractAsyncContextManager`.

   .. versionadded:: 3.5.4
   .. versionadded:: 3.6.2

   .. deprecated:: 3.9
      :class:`contextlib.AbstractAsyncContextManager` now supports ``[]``. See
      :pep:`585` and :ref:`types-genericalias`.

Protocols
---------

These protocols are decorated with :func:`runtime_checkable`.

.. class:: SupportsAbs

    An ABC with one abstract method ``__abs__`` that is covariant
    in its return type.

.. class:: SupportsBytes

    An ABC with one abstract method ``__bytes__``.

.. class:: SupportsComplex

    An ABC with one abstract method ``__complex__``.

.. class:: SupportsFloat

    An ABC with one abstract method ``__float__``.

.. class:: SupportsIndex

    An ABC with one abstract method ``__index__``.

    .. versionadded:: 3.8

.. class:: SupportsInt

    An ABC with one abstract method ``__int__``.

.. class:: SupportsRound

    An ABC with one abstract method ``__round__``
    that is covariant in its return type.

Functions and decorators
------------------------

.. function:: cast(typ, val)

   Cast a value to a type.

   This returns the value unchanged.  To the type checker this
   signals that the return value has the designated type, but at
   runtime we intentionally don't check anything (we want this
   to be as fast as possible).

.. decorator:: overload

   The ``@overload`` decorator allows describing functions and methods
   that support multiple different combinations of argument types. A series
   of ``@overload``-decorated definitions must be followed by exactly one
   non-``@overload``-decorated definition (for the same function/method).
   The ``@overload``-decorated definitions are for the benefit of the
   type checker only, since they will be overwritten by the
   non-``@overload``-decorated definition, while the latter is used at
   runtime but should be ignored by a type checker.  At runtime, calling
   a ``@overload``-decorated function directly will raise
   :exc:`NotImplementedError`. An example of overload that gives a more
   precise type than can be expressed using a union or a type variable::

      @overload
      def process(response: None) -> None:
          ...
      @overload
      def process(response: int) -> tuple[int, str]:
          ...
      @overload
      def process(response: bytes) -> str:
          ...
      def process(response):
          <actual implementation>

   See :pep:`484` for details and comparison with other typing semantics.

.. decorator:: final

   A decorator to indicate to type checkers that the decorated method
   cannot be overridden, and the decorated class cannot be subclassed.
   For example::

      class Base:
          @final
          def done(self) -> None:
              ...
      class Sub(Base):
          def done(self) -> None:  # Error reported by type checker
                ...

      @final
      class Leaf:
          ...
      class Other(Leaf):  # Error reported by type checker
          ...

   There is no runtime checking of these properties. See :pep:`591` for
   more details.

   .. versionadded:: 3.8

.. decorator:: no_type_check

   Decorator to indicate that annotations are not type hints.

   This works as class or function :term:`decorator`.  With a class, it
   applies recursively to all methods defined in that class (but not
   to methods defined in its superclasses or subclasses).

   This mutates the function(s) in place.

.. decorator:: no_type_check_decorator

   Decorator to give another decorator the :func:`no_type_check` effect.

   This wraps the decorator with something that wraps the decorated
   function in :func:`no_type_check`.

.. decorator:: type_check_only

   Decorator to mark a class or function to be unavailable at runtime.

   This decorator is itself not available at runtime. It is mainly
   intended to mark classes that are defined in type stub files if
   an implementation returns an instance of a private class::

      @type_check_only
      class Response:  # private or not available at runtime
          code: int
          def get_header(self, name: str) -> str: ...

      def fetch_response() -> Response: ...

   Note that returning instances of private classes is not recommended.
   It is usually preferable to make such classes public.

Introspection helpers
---------------------

.. function:: get_type_hints(obj, globalns=None, localns=None, include_extras=False)

   Return a dictionary containing type hints for a function, method, module
   or class object.

   This is often the same as ``obj.__annotations__``. In addition,
   forward references encoded as string literals are handled by evaluating
   them in ``globals`` and ``locals`` namespaces. If necessary,
   ``Optional[t]`` is added for function and method annotations if a default
   value equal to ``None`` is set. For a class ``C``, return
   a dictionary constructed by merging all the ``__annotations__`` along
   ``C.__mro__`` in reverse order.

   The function recursively replaces all ``Annotated[T, ...]`` with ``T``,
   unless ``include_extras`` is set to ``True`` (see :class:`Annotated` for
   more information). For example::

       class Student(NamedTuple):
           name: Annotated[str, 'some marker']

       get_type_hints(Student) == {'name': str}
       get_type_hints(Student, include_extras=False) == {'name': str}
       get_type_hints(Student, include_extras=True) == {
           'name': Annotated[str, 'some marker']
       }

   .. versionchanged:: 3.9
      Added ``include_extras`` parameter as part of :pep:`593`.

.. function:: get_args(tp)
.. function:: get_origin(tp)

   Provide basic introspection for generic types and special typing forms.

   For a typing object of the form ``X[Y, Z, ...]`` these functions return
   ``X`` and ``(Y, Z, ...)``. If ``X`` is a generic alias for a builtin or
   :mod:`collections` class, it gets normalized to the original class.
   If ``X`` is a :class:`Union` or :class:`Literal` contained in another
   generic type, the order of ``(Y, Z, ...)`` may be different from the order
   of the original arguments ``[Y, Z, ...]`` due to type caching.
   For unsupported objects return ``None`` and ``()`` correspondingly.
   Examples::

      assert get_origin(Dict[str, int]) is dict
      assert get_args(Dict[int, str]) == (int, str)

      assert get_origin(Union[int, str]) is Union
      assert get_args(Union[int, str]) == (int, str)

   .. versionadded:: 3.8

.. class:: ForwardRef

   A class used for internal typing representation of string forward references.
   For example, ``List["SomeClass"]`` is implicitly transformed into
   ``List[ForwardRef("SomeClass")]``.  This class should not be instantiated by
   a user, but may be used by introspection tools.

   .. note::
      :pep:`585` generic types such as ``list["SomeClass"]`` will not be
      implicitly transformed into ``list[ForwardRef("SomeClass")]`` and thus
      will not automatically resolve to ``list[SomeClass]``.

   .. versionadded:: 3.7.4

Constant
--------

.. data:: TYPE_CHECKING

   A special constant that is assumed to be ``True`` by 3rd party static
   type checkers. It is ``False`` at runtime. Usage::

      if TYPE_CHECKING:
          import expensive_mod

      def fun(arg: 'expensive_mod.SomeType') -> None:
          local_var: expensive_mod.AnotherType = other_fun()

   The first type annotation must be enclosed in quotes, making it a
   "forward reference", to hide the ``expensive_mod`` reference from the
   interpreter runtime.  Type annotations for local variables are not
   evaluated, so the second annotation does not need to be enclosed in quotes.

   .. note::

      If ``from __future__ import annotations`` is used in Python 3.7 or later,
      annotations are not evaluated at function definition time.
      Instead, they are stored as strings in ``__annotations__``,
      This makes it unnecessary to use quotes around the annotation.
      (see :pep:`563`).

   .. versionadded:: 3.5.2

