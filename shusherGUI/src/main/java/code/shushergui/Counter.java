package code.shushergui;

public class Counter {
    private static Counter instance = null;
    private int counter;

    private Counter() {
        // Private constructor to prevent instantiation from outside the class
        counter = 0;
    }

    public static Counter getInstance() {
        if (instance == null) {
            instance = new Counter();
        }
        return instance;
    }

    public int getCounter() {
        return counter;
    }

    public void increment() {
        counter++;
    }



}
