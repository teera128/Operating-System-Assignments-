use std::sync::{Arc, Mutex};
use std::sync::mpsc::{channel, Sender, Receiver};
use std::thread;
use std::time::Duration;

const QUEUE_SIZE: usize = 10;
const NUMBER_OF_THREADS: usize = 3;

type Task = Box<dyn FnOnce() + Send + 'static>;

struct ThreadPool {
    workers: Vec<Worker>,
    sender: Sender<Task>,
}

impl ThreadPool {
    fn new(size: usize) -> ThreadPool {
        assert!(size > 0);

        let (sender, receiver) = channel();
        let receiver = Arc::new(Mutex::new(receiver));

        let mut workers = Vec::with_capacity(size);
        for id in 0..size {
            workers.push(Worker::new(id, Arc::clone(&receiver)));
        }

        ThreadPool { workers, sender }
    }

    fn submit<F>(&self, f: F)
    where
        F: FnOnce() + Send + 'static,
    {
        self.sender.send(Box::new(f)).unwrap();
    }
}

struct Worker {
    id: usize,
    thread: Option<thread::JoinHandle<()>>,
}

impl Worker {
    fn new(id: usize, receiver: Arc<Mutex<Receiver<Task>>>) -> Worker {
        let thread = thread::spawn(move || loop {
            let task = receiver.lock().unwrap().recv().unwrap();
            println!("Worker {} got a job; executing.", id);
            task();
        });

        Worker {
            id,
            thread: Some(thread),
        }
    }
}

impl Drop for ThreadPool {
    fn drop(&mut self) {
        for worker in &mut self.workers {
            println!("Shutting down worker {}", worker.id);

            if let Some(thread) = worker.thread.take() {
                thread.join().unwrap();
            }
        }
    }
}

fn log_ab(a: i32, b: i32) {
    let result = (a * b) as f64;
    println!("log({} * {}) = {}", a, b, result.log10());
}

fn sqrt_ab(a: i32, b: i32) {
    let result = (a * b) as f64;
    println!("sqrt({} * {}) = {}", a, b, result.sqrt());
}

fn hypotenuse(a: i32, b: i32) {
    let result = ((a * a + b * b) as f64).sqrt();
    println!("hypotenuse({}, {}) = {}", a, b, result);
}
