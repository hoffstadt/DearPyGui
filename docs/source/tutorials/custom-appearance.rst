Custom Appearance
=================

.. meta::
   :description lang=en: overview of the different ways to change the appearance of a dpg app.

A DPG app can be customized by:
    * Font
    * Theme

Font
----

.. seealso:: for more information on fonts :doc:`../api-reference/fonts`

Theme
-----

Themes work from a top down inheritance approach.

Themes can be set at the global, container, or individual item levels.

Global Themes
    Applied across all containers and down.

Container Themes
    Applied across all containers and down.

Individual Item Themes
    Applied specifically to the item its attached to.

.. note:: Some UI items in underneath are a composite of more basic UI items
    setting themes at at the container level may affect other
    items that utilize the same theme items. (ex. *mvThemeCol_Button*
    changes the button item and the button on the date picker item)

.. seealso:: for more information on themes :doc:`../api-reference/themes`