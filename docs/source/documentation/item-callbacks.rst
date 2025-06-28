Item Callbacks
==============

Most items have a callback which is submitted to a 
**queue of callbacks** when the item is interacted with.

Callbacks are used to give functionality to items. Callbacks 
can either be assigned to the item upon creation or after creation 
using :py:func:`set_item_callback <dearpygui.dearpygui.set_item_callback>` 
as shown in the code below.

Callbacks in DPG can have up to 3 arguments. The first is usually
the sender or item triggering the callback. The second is data sent by
DPG for different reasons. The third is reserved for user specified data.
We refer to this in general terms as: **sender**, **app_data**, and **user_data**.

.. note:: Because they are optional positional arguments you 
    must use the *sender* and *app_data* if you want to use *user_data*
    keyword arguments:

Sender, App_data
----------------

sender:
    argument is used by DPG to inform the
    callback which item triggered the callback by sending the tag
    or 0 if trigger by the application.

app_data:
    argument is used by DPG to send information
    to the callback i.e. the current value of most basic widgets.

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def button_callback(sender, app_data):
        print(f"sender is: {sender}")
        print(f"app_data is: {app_data}")

    with dpg.window(label="Tutorial"):
        dpg.add_button(label="Print to Terminal", callback=button_callback)

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

User Data
---------

user_data:
    argument is **Optionally** used to pass
    your own python data into the function.

The python data can be assigned or updated to the keyword *user_data* when the
item is created or after the item is created using
:py:func:`set_item_user_data <dearpygui.dearpygui.set_item_user_data>`

User data can be any python object. 

.. code-block:: python

    import dearpygui.dearpygui as dpg

    dpg.create_context()

    def button_callback(sender, app_data, user_data):
        print(f"sender is: {sender}")
        print(f"app_data is: {app_data}")
        print(f"user_data is: {user_data}")

    with dpg.window(label="Tutorial"):
        # user data set when button is created
        dpg.add_button(label="Print to Terminal", callback=button_callback, user_data="Some Data")

        # user data and callback set any time after button has been created
        dpg.add_button(label="Print to Terminal 2", tag="btn")
        dpg.set_item_callback("btn", button_callback)
        dpg.set_item_user_data("btn", "Some Extra User Data")

    dpg.create_viewport(title='Custom Title', width=800, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

Drag and Drop callback (and drag payload)
-----------------------------------------

Drag/Drop callback receive its **app_data** from drag_payload's **drag_data**.

**user_data** (optional) can be specified in both **drag payload** and **target drop item** for further use in Drag/Drop callback.

**payload_type** (optional) can be any string you like. If not specified, **payload_type**'s default value is '$$DPG_PAYLOAD'. 

For drop callback to work, **payload_type** must be specified the same in both **drag payload** and **target drop item**.

.. code-block:: python

    import dearpygui.dearpygui as dpg
    dpg.create_context()
    
    def drag_cb(sender, app_data, user_data):
        # sender is btn_drag
        # app_data is btn_drag (value from drag_data)
        # do some configure(drawing_item), animation
        ...
    
    def drop_cb(sender, app_data, user_data):
        # sender is group, app_data is btn_drag
        dpg.move_item(app_data, parent=sender)
    
    with dpg.window():
        with dpg.group(horizontal=True):
    
            with dpg.group(width=300, drop_callback=drop_cb, payload_type="int"):  # user_data=??
                dpg.add_text("Group left")
                dpg.add_button(label="not drag this")
    
            with dpg.group(width=300, drop_callback=drop_cb, payload_type="int"):
                dpg.add_text("Group right")
                dpg.add_button(label="not drag this")
                btn_drag = dpg.add_button(label="drag me to another group then drop", drag_callback=drag_cb)
    
            with dpg.drag_payload(parent=btn_drag, drag_data=btn_drag, payload_type="int"):
                dpg.add_text("dragging a button")
    
                # parent=btn_drag     --> this playload will appear if dragged from the btn_drag
                # drag_data=btn_drag  --> btn_drag will be app_data in the above drag_cb and drop_cb
                # payload_type="int"  --> btn_drag is an int, specified in this playload and drop target - two group above
                
    dpg.create_viewport()
    dpg.setup_dearpygui()
    dpg.show_viewport()
    while dpg.is_dearpygui_running():
        dpg.render_dearpygui_frame()
    dpg.destroy_context()

Debugging Callbacks (new in 1.2)
--------------------------------

Because callbacks are not run on the main thread, debugging can be a hassle.
In 1.2 we added a few utilities to help with this. 

By default, Dear PyGui handles the callbacks internally on a worker thread. This allows for 
optimizations and steady framerates. However, to help with debugging, you can set the new 
**manual_callback_management** key to **True** with :py:func:`configure_app <dearpygui.dearpygui.configure_app>`. 
This will prevent Dear PyGui from handling the callbacks. Instead the callbacks and arguments will be stored.
You can then retrieve (and clear) them by calling :py:func:`get_callback_queue <dearpygui.dearpygui.get_callback_queue>` within
your main event loop. This will return a list of "Jobs". A "Job" is just list with the first item being the callable and
the remaining items (up to 3) being the typical arguments. We have also provided :py:func:`run_callbacks <dearpygui.dearpygui.run_callbacks>` 
to properly handle the jobs for simple usage.

Below is a simple example

.. code-block:: python

    import dearpygui.dearpygui as dpg
    
    dpg.create_context()
    dpg.configure_app(manual_callback_management=True)
    dpg.create_viewport()
    dpg.setup_dearpygui()
    
    def callback(sender, app_data, user_data):
        print("Called on the main thread!")
    
    with dpg.window(label="Tutorial"):
        dpg.add_button(label="Press me", callback=callback)
    
    
    # main loop
    dpg.show_viewport()
    while dpg.is_dearpygui_running():
        jobs = dpg.get_callback_queue() # retrieves and clears queue
        dpg.run_callbacks(jobs)
        dpg.render_dearpygui_frame()  
    
    dpg.destroy_context()
