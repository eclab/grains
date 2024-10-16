;;; Copyright 2024 Sean Luke
;;; (sean@cs.gmu.edu)
;;;
;;; Released under the Apache 2.0 License
;;;
;;; This is just some code I used to generate the exponential dropoff tables.

(defparameter *x* 0.0)



(setf *x* 256.0)
(dotimes (i 32768)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.99983082))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 32778

(setf *x* 256.0)
(dotimes (i 16384)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.999662))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 16402

(setf *x* 256.0)
(dotimes (i 8192)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.999324))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 8200

(setf *x* 256.0)
(dotimes (i 4096)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.99865))
	(when (= (floor *x*) 0) (return))) 
(terpri)
;; 4104
	
(setf *x* 256.0)
(dotimes (i 2048)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.9973))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 2051
	
(setf *x* 256.0)
(dotimes (i 1024)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.995))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 1106
	
(setf *x* 256.0)
(terpri)
(dotimes (i 512)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.99))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 551

(setf *x* 256.0)
(terpri)
(dotimes (i 256)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.98))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 274

(setf *x* 256.0)
(terpri)
(dotimes (i 128)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.96))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 135

(setf *x* 256.0)
(terpri)
(dotimes (i 64)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.92))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 66

(setf *x* 256.0)
(terpri)
(dotimes (i 32)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.85))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 34

(setf *x* 256.0)
(terpri)
(dotimes (i 16)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.71))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 16

(setf *x* 256.0)
(terpri)
(dotimes (i 100)
	(format t "~a, " (floor *x*))
	(if (= 31 (mod i 32)) (terpri))
	(setf *x* (* *x* 0.50))
	(when (= (floor *x*) 0) (print i) (return))) 
(terpri)
;; 8
