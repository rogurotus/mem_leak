
import 'dart:async';

import 'package:flutter/services.dart';

class MemLeak {
  static const MethodChannel _channel = MethodChannel('mem_leak');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
