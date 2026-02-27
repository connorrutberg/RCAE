public sealed class SelectionBindingController
{
    private readonly ISelectionService _selection;
    private readonly IPropertiesPanel _properties;
    private readonly IScriptCatalog _scriptCatalog;

    public SelectionBindingController(
        ISelectionService selection,
        IPropertiesPanel properties,
        IScriptCatalog scriptCatalog)
    {
        _selection = selection;
        _properties = properties;
        _scriptCatalog = scriptCatalog;

        _selection.Changed += OnSelectionChanged;
    }

    private void OnSelectionChanged(EntityId id)
    {
        var components = _scriptCatalog.GetBindableComponents(id);
        _properties.ShowEntity(id);
        _properties.ShowScriptsSection(components); // searchable + click-to-add
    }
}
