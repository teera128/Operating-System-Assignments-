mod threadpool;

use threadpool::ThreadPool;
use std::time::Duration;

fn main() {
    let pool = ThreadPool::new(3);

    let tasks = vec![
        (5, 10),
        (7, 3),
        (20, 30),
    ];

    for (a, b) in tasks {
        pool.submit(move || {
            threadpool::log_ab(a, b);
            threadpool::sqrt_ab(a, b);
            threadpool::hypotenuse(a, b);
        });
    }

    std::thread::sleep(Duration::from_secs(3));
}
