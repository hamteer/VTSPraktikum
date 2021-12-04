public class FigurenThreads {

    public static void main(String[] args) {
        Queen q = new Queen();
        Leser lthread = new Leser(q);
        Schreiber wthread = new Schreiber(q);

        wthread.setDaemon(true);

        wthread.start();
        MachMal.eineZehntelSekundeLangGarNichts();
        lthread.start();
    }
}
