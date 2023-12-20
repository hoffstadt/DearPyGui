
def deprecated(reason):

	string_types = (type(b''), type(u''))

	if isinstance(reason, string_types):

		def decorator(func1):

			fmt1 = "Call to deprecated function {name} ({reason})."

			@functools.wraps(func1)
			def new_func1(*args, **kwargs):
				warnings.simplefilter('always', DeprecationWarning)
				warnings.warn(
					fmt1.format(name=func1.__name__, reason=reason),
					category=DeprecationWarning,
					stacklevel=2
				)
				warnings.simplefilter('default', DeprecationWarning)
				return func1(*args, **kwargs)

			return new_func1

		return decorator

	elif inspect.isfunction(reason):

		func2 = reason
		fmt2 = "Call to deprecated function {name}."

		@functools.wraps(func2)
		def new_func2(*args, **kwargs):
			warnings.simplefilter('always', DeprecationWarning)
			warnings.warn(
				fmt2.format(name=func2.__name__),
				category=DeprecationWarning,
				stacklevel=2
			)
			warnings.simplefilter('default', DeprecationWarning)
			return func2(*args, **kwargs)

		return new_func2

@deprecated("Use 'configure_app(init_file=file)'.")
def set_init_file(file="dpg.ini"):
    """ deprecated function """
    internal_dpg.configure_app(init_file=file)

@deprecated("Use 'configure_app(init_file=file, load_init_file=True)'.")
def load_init_file(file):
    """ deprecated function """
    internal_dpg.configure_app(init_file=file, load_init_file=True)

@deprecated("Use: `is_viewport_ok(...)`")
def is_viewport_created():
    """ deprecated function """
    return internal_dpg.is_viewport_ok()

@deprecated("Use: \ncreate_viewport()\nsetup_dearpygui()\nshow_viewport()")
def setup_viewport():
    """ deprecated function """
    internal_dpg.create_viewport()
    internal_dpg.setup_dearpygui()
    internal_dpg.show_viewport()

@deprecated("Use: `bind_item_theme(...)`")
def set_item_theme(item, theme):
    """ deprecated function """
    return internal_dpg.bind_item_theme(item, theme)

@deprecated("Use: `bind_item_type_disabled_theme(...)`")
def set_item_type_disabled_theme(item, theme):
    """ deprecated function """
    return internal_dpg.bind_item_type_disabled_theme(item, theme)

@deprecated("Use: `bind_item_type_theme(...)`")
def set_item_type_theme(item, theme):
    """ deprecated function """
    return internal_dpg.bind_item_type_theme(item, theme)

@deprecated("Use: `bind_item_font(...)`")
def set_item_font(item, font):
    """ deprecated function """
    return internal_dpg.bind_item_font(item, font)

@deprecated("Use: `add_item_activated_handler(...)`")
def add_activated_handler(parent, **kwargs):
    """ deprecated function """
    return internal_dpg.add_item_activated_handler(parent, **kwargs)

@deprecated("Use: `add_item_active_handler(...)`")
def add_active_handler(parent, **kwargs):
    """ deprecated function """
    return internal_dpg.add_item_active_handler(parent, **kwargs)

@deprecated("Use: `add_item_clicked_handler(...)`")
def add_clicked_handler(parent, button=-1, **kwargs):
    """ deprecated function """
    return internal_dpg.add_item_clicked_handler(parent, button, **kwargs)

@deprecated("Use: `add_item_deactived_after_edit_handler(...)`")
def add_deactivated_after_edit_handler(parent, **kwargs):
    """ deprecated function """
    return internal_dpg.add_item_deactivated_after_edit_handler(parent, **kwargs)

@deprecated("Use: `add_item_deactivated_handler(...)`")
def add_deactivated_handler(parent, **kwargs):
    """ deprecated function """
    return internal_dpg.add_item_deactivated_handler(parent, **kwargs)

@deprecated("Use: `add_item_edited_handler(...)`")
def add_edited_handler(parent, **kwargs):
    """ deprecated function """
    return internal_dpg.add_item_edited_handler(parent, **kwargs)

@deprecated("Use: `add_item_focus_handler(...)`")
def add_focus_handler(parent, **kwargs):
    """ deprecated function """
    return internal_dpg.add_item_focus_handler(parent, **kwargs)

@deprecated("Use: `add_item_hover_handler(...)`")
def add_hover_handler(parent, **kwargs):
    """ deprecated function """
    return internal_dpg.add_item_hover_handler(parent, **kwargs)

@deprecated("Use: `add_item_resize_handler(...)`")
def add_resize_handler(parent, **kwargs):
    """ deprecated function """
    return internal_dpg.add_item_resize_handler(parent, **kwargs)

@deprecated("Use: `add_item_toggled_open_handler(...)`")
def add_toggled_open_handler(parent, **kwargs):
    """ deprecated function """
    return internal_dpg.add_item_toggled_open_handler(parent, **kwargs)

@deprecated("Use: `add_item_visible_handler(...)`")
def add_visible_handler(parent, **kwargs):
    """ deprecated function """
    return internal_dpg.add_item_visible_handler(parent, **kwargs)

@deprecated("Use: `bind_colormap(...)`")
def set_colormap(item, source):
    """ deprecated function """
    return internal_dpg.bind_colormap(item, source)

@deprecated("Use: `bind_theme(0)`")
def reset_default_theme(item, source):
    """ deprecated function """
    return internal_dpg.bind_theme(item, source)

@deprecated
def set_staging_mode(mode):
    """ deprecated function """
    pass

@deprecated
def add_table_next_column(**kwargs):
    """ deprecated function """
    pass

@deprecated("Use: add_stage")
def add_staging_container(**kwargs):
    """ deprecated function """
    return internal_dpg.add_stage(**kwargs)

@deprecated("Use: stage")
@contextmanager
def staging_container(**kwargs):
	"""
	deprecated function
	Args:
		**label (str): Overrides 'name' as label.
		**user_data (Any): User data for callbacks.
		**use_internal_label (bool): Use generated internal label instead of user specified (appends ### uuid).
		**id (Union[int, str]): Unique id used to programmatically refer to the item.If label is unused this will be the label.
	Yields:
		Union[int, str]
	"""
	try:
		warnings.warn("'staging_container' is deprecated and was changed to 'stage'", DeprecationWarning, 2)
		widget = internal_dpg.add_stage_container(**kwargs)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()

@deprecated("Use: add_spacer(...)")
def add_spacing(**kwargs):
	"""	(deprecated function) Adds vertical spacing. 

	Args:
		label (str, optional): Overrides 'name' as label.
		user_data (Any, optional): User data for callbacks.
		use_internal_label (bool, optional): Use generated internal label instead of user specified (appends ### uuid).
		tag (Union[int, str], optional): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		indent (int, optional): Offsets the widget to the right the specified number multiplied by the indent style.
		parent (Union[int, str], optional): Parent to add this item to. (runtime adding)
		before (Union[int, str], optional): This item will be displayed before the specified item in the parent.
		show (bool, optional): Attempt to render widget.
		pos (Union[List[int], Tuple[int]], optional): Places the item relative to window coordinates, [0,0] is top left.
		count (int, optional): Number of spacings to add the size is dependant on the curret style.
	Returns:
		Union[int, str]
	"""

	if 'count' in kwargs.keys():
		count = kwargs["count"]
		kwargs.pop("count", None)
		internal_dpg.add_group(**kwargs)
		internal_dpg.push_container_stack(internal_dpg.last_container())
		for i in range(count):
			internal_dpg.add_spacer()
		result_id = internal_dpg.pop_container_stack()
	else:
		result_id = internal_dpg.add_spacer(**kwargs)
	return result_id

@deprecated("Use: add_spacer(...)")
def add_dummy(**kwargs):
	"""	(deprecated function) Adds a spacer or 'dummy' object.

	Args:
		label (str, optional): Overrides 'name' as label.
		user_data (Any, optional): User data for callbacks.
		use_internal_label (bool, optional): Use generated internal label instead of user specified (appends ### uuid).
		tag (Union[int, str], optional): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		width (int, optional): Width of the item.
		height (int, optional): Height of the item.
		indent (int, optional): Offsets the widget to the right the specified number multiplied by the indent style.
		parent (Union[int, str], optional): Parent to add this item to. (runtime adding)
		before (Union[int, str], optional): This item will be displayed before the specified item in the parent.
		show (bool, optional): Attempt to render widget.
		pos (Union[List[int], Tuple[int]], optional): Places the item relative to window coordinates, [0,0] is top left.
	Returns:
		Union[int, str]
	"""

	return internal_dpg.add_spacer(**kwargs)

@deprecated("Use: `destroy_context()`")
def cleanup_dearpygui():
    """ deprecated function """
    return internal_dpg.destroy_context()

@deprecated("Use: group(horizontal=True)")
def add_same_line(**kwargs):
    """ deprecated function """

    last_item = internal_dpg.last_item()
    group = internal_dpg.add_group(horizontal=True, **kwargs)
    internal_dpg.move_item(last_item, parent=group)
    internal_dpg.capture_next_item(lambda s: internal_dpg.move_item(s, parent=group))
    return group


@deprecated("Use: `add_child_window()`")
def add_child(**kwargs):
	"""	(deprecated function) Adds an embedded child window. Will show scrollbars when items do not fit.

	Args:
		label (str, optional): Overrides 'name' as label.
		user_data (Any, optional): User data for callbacks
		use_internal_label (bool, optional): Use generated internal label instead of user specified (appends ### uuid).
		tag (Union[int, str], optional): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		width (int, optional): Width of the item.
		height (int, optional): Height of the item.
		indent (int, optional): Offsets the widget to the right the specified number multiplied by the indent style.
		parent (Union[int, str], optional): Parent to add this item to. (runtime adding)
		before (Union[int, str], optional): This item will be displayed before the specified item in the parent.
		payload_type (str, optional): Sender string type must be the same as the target for the target to run the payload_callback.
		drop_callback (Callable, optional): Registers a drop callback for drag and drop.
		show (bool, optional): Attempt to render widget.
		pos (Union[List[int], Tuple[int]], optional): Places the item relative to window coordinates, [0,0] is top left.
		filter_key (str, optional): Used by filter widget.
		delay_search (bool, optional): Delays searching container for specified items until the end of the app. Possible optimization when a container has many children that are not accessed often.
		tracked (bool, optional): Scroll tracking
		track_offset (float, optional): 0.0f:top, 0.5f:center, 1.0f:bottom
		border (bool, optional): Shows/Hides the border around the sides.
		autosize_x (bool, optional): Autosize the window to its parents size in x.
		autosize_y (bool, optional): Autosize the window to its parents size in y.
		no_scrollbar (bool, optional):  Disable scrollbars (window can still scroll with mouse or programmatically).
		horizontal_scrollbar (bool, optional): Allow horizontal scrollbar to appear (off by default).
		menubar (bool, optional): Shows/Hides the menubar at the top.
	Returns:
		Union[int, str]
	"""

	return internal_dpg.add_child_window(**kwargs)


@deprecated("Use: `child_window()`")
@contextmanager
def child(**kwargs):
	"""	(deprecated function) Adds an embedded child window. Will show scrollbars when items do not fit.

	Args:
		label (str, optional): Overrides 'name' as label.
		user_data (Any, optional): User data for callbacks
		use_internal_label (bool, optional): Use generated internal label instead of user specified (appends ### uuid).
		tag (Union[int, str], optional): Unique id used to programmatically refer to the item.If label is unused this will be the label.
		width (int, optional): Width of the item.
		height (int, optional): Height of the item.
		indent (int, optional): Offsets the widget to the right the specified number multiplied by the indent style.
		parent (Union[int, str], optional): Parent to add this item to. (runtime adding)
		before (Union[int, str], optional): This item will be displayed before the specified item in the parent.
		payload_type (str, optional): Sender string type must be the same as the target for the target to run the payload_callback.
		drop_callback (Callable, optional): Registers a drop callback for drag and drop.
		show (bool, optional): Attempt to render widget.
		pos (Union[List[int], Tuple[int]], optional): Places the item relative to window coordinates, [0,0] is top left.
		filter_key (str, optional): Used by filter widget.
		delay_search (bool, optional): Delays searching container for specified items until the end of the app. Possible optimization when a container has many children that are not accessed often.
		tracked (bool, optional): Scroll tracking
		track_offset (float, optional): 0.0f:top, 0.5f:center, 1.0f:bottom
		border (bool, optional): Shows/Hides the border around the sides.
		autosize_x (bool, optional): Autosize the window to its parents size in x.
		autosize_y (bool, optional): Autosize the window to its parents size in y.
		no_scrollbar (bool, optional):  Disable scrollbars (window can still scroll with mouse or programmatically).
		horizontal_scrollbar (bool, optional): Allow horizontal scrollbar to appear (off by default).
		menubar (bool, optional): Shows/Hides the menubar at the top.
	Yields:
		Union[int, str]
	"""
	try:
		widget = internal_dpg.add_child_window(**kwargs)
		internal_dpg.push_container_stack(widget)
		yield widget
	finally:
		internal_dpg.pop_container_stack()

@deprecated("Use: Just not recommended")
def setup_registries() -> None:
    """Adds default registries for fonts, handlers, textures, colormaps, and values."""
    internal_dpg.add_font_registry(tag=internal_dpg.mvReservedUUID_0)
    internal_dpg.add_handler_registry(tag=internal_dpg.mvReservedUUID_1)
    internal_dpg.add_texture_registry(tag=internal_dpg.mvReservedUUID_2)
    internal_dpg.add_value_registry(tag=internal_dpg.mvReservedUUID_3)
    internal_dpg.add_colormap_registry(tag=internal_dpg.mvReservedUUID_4)

@deprecated("Use: `set_frame_callback()`")
def set_start_callback(callback):
    """ deprecated function """
    return internal_dpg.set_frame_callback(3, callback)
