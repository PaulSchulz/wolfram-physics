(ns wolfram-physics.core
  (:gen-class))

(defn -main
  "I don't do a whole lot ... yet."
  [& args]
  (println "Hello, World!"))

(defn next-block [n block]
  (let [[a b c] block]
    (case n
      :a [(inc a) b c]
      :b [a (inc b) c]
      :c [a b (inc c)]
      )
    )
  )

(defn next-block-a [block] (next-block :a block))
(defn next-block-b [block] (next-block :b block))
(defn next-block-c [block] (next-block :c block))

(defn support-block [s n block]
  (let [[a b c] block]
    (case n
      :a (if (= a 0) true
             (contains? s [(dec a) b c]))
      :b (if (= b 0) true
             (contains? s [a (dec b) c]))
      :c (if (= c 0) true
             (contains? s [a b (dec c)]))
      )
    )
  )


(def arr '([0 0 0]))
(def s   (set arr))

(def new-s
  (conj s
        (next-block :a [0 0 0])
        (next-block :b [0 0 0])
        (next-block :c [0 0 0])
        ))

(defn new-blocks
  [s]
  (apply reduce s (map next-block-a s)))
