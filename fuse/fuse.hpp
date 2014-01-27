#pragma once

#include <type_traits>
#include <tuple>
#include <string>

namespace fuse {
  /**
   * Represents a boolean value.
   *
   * @tparam T The boolean value
   */
  template <bool T>
  struct boolean : std::conditional<T, std::true_type, std::false_type>::type { };

  /**
   * Represents the base class for all sequences.
   */
  struct base_seq { };

  /**
   * Represents an empty sequence.
   */
  struct empty_seq : base_seq { };

  /**
   * Represents a sequence of types.
   *
   * @tparam Head The head of the list
   * @tparam Tail A sequence containing tail (rest) of the sequence.
   */
  template <class Head, class Tail = empty_seq>
  struct seq : base_seq {
    static_assert(std::is_base_of<base_seq, Tail>::value,
        "Tail parameter of sequence must be a sequence");

    /**
     * The first element in the sequence.
     */
    using head = Head;

    /**
     * A sequence containing the rest of the sequence.
     */
    using tail = Tail;
  };

  /**
   * Helper function to create a sequence from variadic parameters.
   * Any empty sequences in the parameters are ignored.
   *
   * @tparam Head The first element in the sequence
   * @tparam Args The rest of the elements
   */
  template <class Head, class...Args>
  struct make_seq {
    /**
     * The type of the created sequence.
     */
    using type = typename std::conditional<
      std::is_same<Head, empty_seq>::value,
      typename make_seq<Args...>::type,
      seq<Head, typename make_seq<Args...>::type>
    >::type;
  };

  /**
   * Base case specialisation for make_seq.
   */
  template <>
  struct make_seq<empty_seq> {
    using type = empty_seq;
  };

  /**
   * Base case specialisation for make_seq.
   */
  template <class Head>
  struct make_seq<Head> {
    using type = seq<Head, empty_seq>;
  };

  /**
   * Checks whether two sequences are equal.
   *
   * @tparam Seq1, Seq2 The two sequences to compare
   */
  template <class Seq1, class Seq2>
  struct equal {
    /**
     * A boolean to indicate whether the two sequences are equal.
     */
    static constexpr bool value =
      std::is_same<typename Seq1::head, typename Seq2::head>::value &&
      equal<typename Seq1::tail, typename Seq2::tail>::value;

    /**
     * A type indicating whether the two sequences are equal.
     */
    using type = boolean<value>;
  };

  /**
   * Base case specialisation for equal. Two empty sequences are equal.
   */
  template <>
  struct equal<empty_seq, empty_seq> : std::true_type { };

  /**
   * Base case specialisation for equal. An empty sequence and
   * a non-empty sequences are unequal.
   */
  template <class Seq1>
  struct equal<Seq1, empty_seq> : std::false_type { };

  /**
   * Base case specialisation for equal. An empty sequence and
   * a non-empty sequences are unequal.
   */
  template <class Seq2>
  struct equal<empty_seq, Seq2> : std::false_type { };

  /**
   * Get the size of a sequence.
   *
   * @tparam Seq The sequence to get the size of
   */
  template <class Seq>
  struct size {
    /**
     * The size of the sequence.
     */
    static constexpr std::size_t value = size<typename Seq::tail>::value + 1;
    
    /**
     * A type containing the size of the sequence.
     */
    using type = std::integral_constant<std::size_t, value>;
  };

  /**
   * Base case specialisation for size. Size of an empty sequence is 0.
   */
  template <>
  struct size<empty_seq> {
    static constexpr std::size_t value = 0;
    using type = std::integral_constant<std::size_t, value>;
  };

  /**
   * Concatenate two sequences together.
   *
   * @tparam Seq1, Seq2 The sequences to concatenate
   */
  template <class Seq1, class Seq2>
  struct concat {
    /**
     * The type of the concatenated lists.
     */
    using type = seq<typename Seq1::head, typename concat<typename Seq1::tail, Seq2>::type>;
  };

  /**
   * Base case specialisation for concat. Concatenating two empty sequences
   * gives an empty sequence.
   */
  template <>
  struct concat<empty_seq, empty_seq> {
    using type = empty_seq;
  };

  /**
   * Base case specialisation for concat.
   */
  template <class Seq2>
  struct concat<empty_seq, Seq2> {
    using type = Seq2;
  };

  /**
   * Base case specialisation for concat.
   */
  template <class Seq1>
  struct concat<Seq1, empty_seq> {
    using type = Seq1;
  };

  /**
   * Get the element at a specific index.
   * 
   * @tparam Seq The sequence
   * @tparam Index The index of the element in the sequence. The index can
   *         be a negative number, which would count from the end of the sequence.
   */
  template <class Seq, int Index> 
  struct get {
    static_assert(!std::is_same<Seq, empty_seq>::value, "Index out of bounds");

    /**
     * The element at the given index.
     */
    using type = typename std::conditional<
      (Index > 0),
      get<typename Seq::tail, Index - 1>,
      get<Seq, size<Seq>::value + Index>
    >::type::type;
  };

  /**
   * Base case specialisation for get.
   */
  template <class Seq>
  struct get<Seq, 0> {
    using type = typename Seq::head;
  };
}
