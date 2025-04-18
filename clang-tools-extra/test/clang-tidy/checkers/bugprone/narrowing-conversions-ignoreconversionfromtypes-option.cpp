// RUN: %check_clang_tidy -check-suffix=DEFAULT %s \
// RUN: bugprone-narrowing-conversions %t --

// RUN: %check_clang_tidy -check-suffix=IGNORED %s \
// RUN: bugprone-narrowing-conversions %t -- \
// RUN: -config='{CheckOptions: { \
// RUN:   bugprone-narrowing-conversions.IgnoreConversionFromTypes: "global_size_t;nested_size_type;long" \
// RUN: }}'

// We use global_size_t instead of 'size_t' because windows predefines size_t.
typedef long long global_size_t;

struct vector {
  typedef long long nested_size_type;

  global_size_t size() const { return 0; }
};

void narrowing_global_size_t() {
  int i;
  global_size_t j;
  i = j;
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:7: warning: narrowing conversion from 'global_size_t' (aka 'long long') to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]
  // IGNORED: Warning is disabled with IgnoreConversionFromTypes=global_size_t.
}

void narrowing_size_type() {
  int i;
  vector::nested_size_type j;
  i = j;
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:7: warning: narrowing conversion from 'vector::nested_size_type' (aka 'long long') to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]
  // IGNORED: Warning is disabled with IgnoreConversionFromTypes=nested_size_type.
}

void narrowing_size_method() {
  vector v;
  int i, j;
  i = v.size();
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:7: warning: narrowing conversion from 'global_size_t' (aka 'long long') to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]
  // IGNORED: Warning is disabled with IgnoreConversionFromTypes=global_size_t.

  i = j + v.size();
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:7: warning: narrowing conversion from 'global_size_t' (aka 'long long') to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]
  // IGNORED: Warning is disabled with IgnoreConversionFromTypes=global_size_t.
}

void narrowing_size_method_binary_expr() {
  int i;
  int j;
  vector v;
  i = j + v.size();
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:7: warning: narrowing conversion from 'global_size_t' (aka 'long long') to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]
  // IGNORED: Warning is disabled with IgnoreConversionFromTypes=global_size_t.
}

void narrowing_size_method_binary_op() {
  int i, j;
  vector v;
  i += v.size();
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:8: warning: narrowing conversion from 'global_size_t' (aka 'long long') to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]
  // IGNORED: Warning is disabled with IgnoreConversionFromTypes=global_size_t.

  i += j + v.size();
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:8: warning: narrowing conversion from 'global_size_t' (aka 'long long') to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]
  // IGNORED: Warning is disabled with IgnoreConversionFromTypes=global_size_t.
}

void most_narrowing_is_not_ok() {
  int i;
  long long j;
  i = j;
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:7: warning: narrowing conversion from 'long long' to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]
  // CHECK-MESSAGES-IGNORED: :[[@LINE-2]]:7: warning: narrowing conversion from 'long long' to signed type 'int' is implementation-defined [bugprone-narrowing-conversions]
}

void test_ignore_builtin_type_pr58809() {
  long x = 123;
  short y = x;
  // CHECK-MESSAGES-DEFAULT: :[[@LINE-1]]:13: warning: narrowing conversion from 'long' to signed type 'short' is implementation-defined [bugprone-narrowing-conversions]
  // CHECK-MESSAGES-NOT-IGNORED: :[[@LINE-2]]:13: warning: narrowing conversion from 'long' to signed type 'short' is implementation-defined [bugprone-narrowing-conversions]
}
