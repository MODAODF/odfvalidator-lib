<?php

if ($argc < 2) {
    die("Usage: php testDll.php <file-path>\n");
}

$filePath = $argv[1];

// 加載 FFI 擴展，指定完整的共享庫路徑
$ffi = FFI::cdef("
    typedef struct ODFValidator ODFValidator;

    ODFValidator* ODFValidator_new();
    void ODFValidator_delete(ODFValidator* validator);
    const char* ODFValidator_check(ODFValidator* validator, const char* filePath);
    const char* ODFValidator_getJsonResult(ODFValidator* validator);
", "/usr/lib/libodfvalidator.so");

// 創建 ODFValidator 實例
$validator = $ffi->ODFValidator_new();

if ($validator === null) {
    die("Failed to create ODFValidator instance.");
}

// 調用 check 方法
$result = $ffi->ODFValidator_check($validator, $filePath);

// 輸出結果
echo "Result: " . $result . PHP_EOL;

// 刪除 ODFValidator 實例
$ffi->ODFValidator_delete($validator);

?>