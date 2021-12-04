
import java.io.IOException;

public class Test {

    public static void main(String[] args) {
        UniqueId uid = new UniqueId("id.dat");

        try {
            uid.init(10000);
        } catch (IOException e) {
            e.printStackTrace();
        }

        Thread[] threads = new Thread[5];

        for (int i = 0; i < 5; i++) {
            threads[i] = new Thread(new Runnable() {
                @Override
                public void run() {
                    for (int j = 0; j < 10; j++) {
                        try {
                            int id = uid.getNext();
                            System.out.println("Thread with hashCode " + this.hashCode() + " returns ID " + id);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }
            });
            threads[i].start();
        }
    }
}
