public class Queen extends Figur{

    @Override
    public synchronized void setPosition(char x, int y) {
        this.x = x;
        MachMal.eineZehntelSekundeLangGarNichts();
        this.y = y;
    }

    @Override
    public synchronized String getPosition() {
        MachMal.eineZehntelSekundeLangGarNichts();
        return "Position: " + this.x + ", " + this.y;
    }
}
