# Twister
Welcome to Twister! Twister is a lightweight tool designed to test the performance of a wireless UDP connection. This is done by generating a pseudorandom test stream at TX using the Mersenne Twister (MT), sending it to RX, and checking for dropped data at RX using the same sequence.

Twister comes with 3 binaries: **rx**, **tx**, and **trace**. For more info on usage, run **(binary) --help**.

## rx
**rx** receives an MT sequence from **tx** while comparing it to the same MT sequence generated at RX, checking for packet drops. The RX MT sequence is generated on a per-packet basis, detecting drops by iterating through the sequence until the RX "packet" matches what was received via TX. The sequence is never explicitly stored except in these small chunks.

## tx
**tx** fills packets with its MT sequence and sends them to **rx**. The send rate is customizable.

## trace
**trace** generates a randomized trace to be used with the **mm-trace** functionality of [this fork of mahimahi](https://github.com/novel-scheduler/mahimahi). The trace describes the packet-by-packet behavior of the mahimahi shell, where 0 represents a transmission and 1 represents a drop. The key parameter of **trace** is the rate *r*, which creates the trace as a series of Bernoulli random variables where P{X = 1} = 1/*r*.

## Building Twister
Run `make` within the `twister/twister` directory, which will output all 3 binaries in the `build` folder.

## Sample Use
The following is a sample use case involving all features of Twister.

1. Generate a trace using Bernoulli distrubtion with *r* = 20. This trace will repeat every 25,000 packets.

        ~/twister/twister/build$ ./trace -l 25000 -r 20

2. Start RX.

        ~/twister/twister/build$ ./rx
        Waiting for TX client...


3. In another window, create a mahimahi shell defined by the trace created in Step 1. Be sure you've installed mahimahi before doing this.

        ~/twister/twister/build$ sudo sysctl net.ipv4.ip_forward=1
        ~/twister/twister/build$ mm-trace uplink trace.txt
        [trace up=trace.txt] ~/twister/twister/build$

4. Start TX at the default send rate of 10 Mb/s.

        [trace up=trace.txt] ~/twister/twister/build$ ./tx
        Creating socket binding for RX...
        Transmitting data to RX...

5. Let RX run for a while, then keyboard interrupt to stop it and see the results. Note that the loss rate is approximately 1/20 and the perceived send rate is almost exactly 10 Mb/s.

        ~/twister/twister/build$ ./rx
        Waiting for TX client...
        Receiving data from TX...
        Writing packet trace to file './out.txt'...
        ^C
        -------------------------
        Twister RX Metrics Report
        -------------------------
        Packets received: 49327
        Packets dropped: 2496
        Loss rate: 4.81639%
        Perceived total packets sent: 51823
        Time elapsed: 49.75363
        Receive rate: 9.51768 Mb/s
        Perceived send rate: 9.99929 Mb/s
        -------------------------



## Future Work
Twister has plenty of room for further development, particularly these features:
- Detecting packet corruptions, along with drops.
- Generating traces with added representation for corruptions.
- Generating traces using different random methods (e.g. Markov chains).

## Notes and Acknowledgements
Twister was created to satisfy the independent research requirement for Rutgers CS 493/494, as well as the Departmental Honors option for the Rutgers Honors College Capstone.

Both Twister and the modifications to mahimahi were created under the advisement of and with help from Prof. Srinivas Naryana from the Rutgers Department of Computer Science.

More information on the Mersenne Twister algorithm: https://en.wikipedia.org/wiki/Mersenne_Twister
