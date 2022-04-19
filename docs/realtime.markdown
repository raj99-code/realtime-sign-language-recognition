---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page
title: Real-Time Performance
permalink: /realtime/

---
Using multithreading, we have separated three stages into separate threads. The grabber thread grabs the frames from the video feed and puts them in a grabber queue. The inference thread processes these frames and detects and recognises the ASL hand symbols. The main thread displays the output.

We executed 50 trial runs of these threads to measure the latency of each of the threads using C++ chrono library. The average latency of the grabber thread was 60.59ms and the inference thread was 850.72ms. And comparatively, the main thread delivered the information faster with the latency of 50.13ms. This means the grabber thread grabs the images and puts them into a grabber queue quite quickly. Then the inference thread for detection and recognition puts them into an inference queue at a slow rate. This results in piling up of the grabber queue.

To solve this issue, we implemented a solution so as to delay the grabber thread. We introduced a sleep time of 700ms for the grabber thread so that it will send the images at a slower rate. This time allows the inference thread to process the images in the queue before it piles up. Subsequently, the main thread displays the output quickly.

The real-time aspect of the project, is however compromised due to this, as the inference takes a lot of time to process. Given a faster inference model, we could achieve a better result.