void core_test() {
  STATIC_TEST("CanCreateSingletonSeq", (
    std::is_same<fuse::seq<int>::head, int>::value &&
    std::is_same<fuse::seq<int>::tail, fuse::empty_seq>::value
  ));

  STATIC_TEST("CanCreateTwoSeq", (
    std::is_same<fuse::seq<int, fuse::seq<char>>::head, int>::value &&
    std::is_same<fuse::seq<int, fuse::seq<char>>::tail, fuse::seq<char>>::value &&
    std::is_same<fuse::seq<int, fuse::seq<char>>::tail::head, char>::value &&
    std::is_same<fuse::seq<int, fuse::seq<char>>::tail::tail, fuse::empty_seq>::value
  ));

  STATIC_TEST("CanMakeSeq", (
    std::is_same<fuse::make_seq<int, char, bool, short>::type::head, int>::value &&
    std::is_same<fuse::make_seq<int, char, bool, short>::type::tail::head, char>::value &&
    std::is_same<fuse::make_seq<int, char, bool, short>::type::tail::tail::head, bool>::value &&
    std::is_same<fuse::make_seq<int, char, bool, short>::type::tail::tail::tail::head, short>::value
  ));

  STATIC_TEST("EmptySeqsAreIgnored", (
    std::is_same<
      fuse::make_seq<fuse::empty_seq, int, fuse::empty_seq, bool, fuse::empty_seq>::type,
      fuse::make_seq<int, bool>::type
    >::value
  ));

  STATIC_TEST("EmptySeqsAreEqual", (
    fuse::equal<fuse::empty_seq, fuse::empty_seq>::value
  ));

  STATIC_TEST("SeqsWithDifferentLengthsAreUnequal", (
    !fuse::equal<fuse::make_seq<int, char, bool>::type, fuse::make_seq<int, char>::type>::value &&
    !fuse::equal<fuse::make_seq<int, char>::type, fuse::make_seq<int, char, bool>::type>::value
  ));
  
  STATIC_TEST("EqualSeqsAreEqual", (
    fuse::equal<fuse::make_seq<int, char, bool>::type, fuse::make_seq<int, char, bool>::type>::value
  ));

  STATIC_TEST("SizeOfEmptySeqIsZero", (
    fuse::size<fuse::empty_seq>::value == 0
  ));

  STATIC_TEST("CanConcatEmptySeqs", (
    std::is_same<fuse::empty_seq, fuse::concat<fuse::empty_seq, fuse::empty_seq>::type>::value
  ));

  STATIC_TEST("CanAppendToEmptySeq", (
    fuse::equal<
      fuse::concat<fuse::empty_seq, fuse::make_seq<int, char>::type>::type,
      fuse::make_seq<int, char>::type
    >::value
  ));

  STATIC_TEST("CanAppendWithEmptySeq", (
    fuse::equal<
      fuse::concat<fuse::make_seq<int, char>::type, fuse::empty_seq>::type,
      fuse::make_seq<int, char>::type
    >::value
  ));

  STATIC_TEST("CanConcatSeqs", (
    fuse::equal<
      fuse::concat<fuse::make_seq<int, char>::type, fuse::make_seq<bool, short>::type>::type,
      fuse::make_seq<int, char, bool, short>::type
    >::value
  ));
}
