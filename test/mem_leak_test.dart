import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:mem_leak/mem_leak.dart';

void main() {
  const MethodChannel channel = MethodChannel('mem_leak');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await MemLeak.platformVersion, '42');
  });
}
