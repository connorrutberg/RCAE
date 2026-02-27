using RCAE.Scripting;

public class DoorTrigger : EntityScript
{
    [ExposeProperty("Target Door", "reference")]
    public EntityRef TargetDoor { get; set; }

    [ExposeProperty("Open Delay", "float")]
    public float OpenDelay { get; set; } = 0.25f;

    public override void OnInteract(EntityRef interactor)
    {
        Scheduler.Delay(OpenDelay, () =>
        {
            TargetDoor.Send("Open");
        });
    }
}
