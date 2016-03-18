Coding style
============

Use tabs for indenting, no space.

Variable naming
===============

All symbols must be in snake case.
Caps are reserved for C macros.

Prefix:
  - 'p' for pointers, a p for each indirection
  - 'c' for constant variables

Functions
=========

Some loose guidelines to help keeping consistency accross the project.

Naming convention
-----------------

It is expressed using EBNF notation.

        prefix                      = 'zob'
        attribute_name              = letter+
        access_verb                 = 'set' | 'get'
        action_verb                 = access_verb | letter+

        fn_action_name              = prefix, '_', action_verb
        fn_attribute_access_name    = prefix, '_', attribute_name, '_', access_type

        fn_name                     = fn_action_name | fn_attribute_access_name


Argument order
--------------

Arguments should appear in that order: target, in, out
The function name should hint the argument list.

Return type
-----------

For now, no decision has been made to ban function returning.
