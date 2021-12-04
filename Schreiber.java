public class Schreiber extends Thread{
    Figur f;

    public Schreiber (Figur f) {
        this.f = f;
    }

    public void run () {
        while (true) {
            int z = (int) (Math.random() * 8);
            char x = (char) ('A' + z);
            int y = 1 + z;
            f.setPosition(x, y);
        }
    }
}
