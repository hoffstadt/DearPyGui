Item Configuration
==================

In DPG various configuration
options and flags can be set when items are created.
There are several options common
to all items (i.e. **show**) but most UI items have specific options.

In order to modify an item's configuration after being created, you can
use the |configure_item| command in conjunction with the keyword from
the item's *add_\*\*\** command. You can also retrieve an item's configuration
in the form of a dictionary by using the |get_item_configuration| command.

.. |get_item_configuration|
    :py:func:`get_item_configuration <dearpygui.dearpygui.get_item_configuration>`

.. |configure_item|
    :py:func:`configure_item <dearpygui.dearpygui.configure_item>`

Example
-------

Simple usage can be found below:

.. code-block:: python

    add_button(enabled=True, label="Press me", id="item")

    # at a later time, change the item's configuration
    configure_item("item", enabled=False, label="New Label")