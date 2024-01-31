/*
   Copyright 2020 Raphael Beck

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdio.h>
#include <string.h>
#include "l8w8jwt/decode.h"

static const char RSA_PRIVATE_KEY[] = "-----BEGIN RSA PRIVATE KEY-----\n"
                                      "MIIJJwIBAAKCAgEAoWFe7BbX1nWo5oaSv/JvIUCWsk/Vi2q8P0cGkefgN5J7MN7K\n"
                                      "fv7lq0hl/1cZcJs81IC+GiC+V3aR2zLBNnJJaxa4sqk+hF5DJcD2bF0B80uqPYQU\n"
                                      "XlQwki/heATnVcke8APuY0kOZykxoD0APAqw0z5KDqgt2vA9G6keM6b9bbL+IvxM\n"
                                      "+yMk1QV0OQLh6Rkz46DyPSoUFWyXiist47PJKNyZAfFZx6vEivzBmqRHKe11W9oD\n"
                                      "/tN5VTQCH/UTSRfyWq/UUMFVMCksLwT6XoWI7F5swgQkSahWkVJ93Qf8cUf1HIZY\n"
                                      "TMJBYPG4y2NDZ0+ytnH3BNXLMQXg9xbgv6B/iaSVScI4CWIpQTAtNKnJwYg2+Rhf\n"
                                      "YBC07iM56c4a+TjbCWgmd11UYc96dbw83uFRjKZc3+SC38ITCgMuoDPNBlFJK6u8\n"
                                      "VfYylGEJolGcauVa6yZKwzsJGr5J/LANz+ZyHZmANed+2Hjqxu/H1NGDBdvUGLQb\n"
                                      "hb/uBJ8oG8iAW5eUyjEJMX0RuncYnBrUjZdEFr0zJd5VkrfFTd26AjGusbiBevAT\n"
                                      "fj83SNa9uK3N3lSNcLNyNXUjmfOU21NWHAk5QV3TJb6SCTcqWFaYoyKR7H6zxRcA\n"
                                      "rNuIAMW4KhOl4jdNnTxJllC4tr/gkE+uO1ntB9ymLxQBRp8osHjuZpKXr3cCAwEA\n"
                                      "AQKCAgAXtQGoRgzMDPnUb6WEPB2WMXJR3Id+1R21X/43lewqzcJ6Ieh2coSTvm15\n"
                                      "bramg6+Seh0zImdD2v+/Rzv5/x0I9cwJNvKfqGdN1wR8U8dzEcT/B3Wki9Kczxrc\n"
                                      "sj+3qvV4BePRwwwyHGuVYhC0QU/LoIVplMwzswIPG697oAbvxBEwW4cFh5qkDoqN\n"
                                      "y34ba5/jSyP610EfCpZSblht8F3XOlzh2644NmQHlOzuBj8MCj2o0iSvHSrgWOUN\n"
                                      "A8gi/zkTmGvktxoIGqxKdf0/wHcmXhK1B7268ldRPuCNhVxQ2eTInXXARPMsxiXC\n"
                                      "/yCKPzt+MMy8cZnJaFcthTdb/zxs5CKKgBKIl5svSW3ZO27SXJ8jo8m0cUUxwAQO\n"
                                      "jJCNroBMBlCDN/sILhpzgnPLkVXnK+/uUYSBe/oOHd7mL38ohlMPepbCeFu4r6fP\n"
                                      "xpOrROTIzDblg9/cne/TLqSPu2K6qbsXFoL3v95V9ieAETnGkTHuKwqW1gMYtOin\n"
                                      "Ad0GWIl4PZCAjbKptSFTM5/8nWiPdJ3YnkE0nQDSgK66ZRjyRCVSzE5CcgbiVlMW\n"
                                      "mXVsIXnt/RHsLsPuGuzURhSmjVg+x0g2nAPmnZuG+7wJOF1vVCYT1gufblaHEBAo\n"
                                      "ofcmYfdYxhd2iQmWJi/uwnC0f6YdF+wq+fLFuPMPgCMvdhe/oQKCAQEA6p8BV4qA\n"
                                      "aRbcV137k4/+jx3zrOA736kRjmHatR1fq0MDTk4JfvWrH5kfl8VS6b4fiYsDVTx6\n"
                                      "ylxYV1paqRUfNmfzuPQStM+arkFIw/6754CoESvqF/uU/JRe6WZIB1Jr3A4XNzZe\n"
                                      "p/9+0hJeQlwRUWPhWMUNIJPjaQ3kmdeIUEsRcIkf4e4xJzhilnxarK4/nkqicgo6\n"
                                      "H2JRD1QKtO96ncGDDLmwWXMxYA9RUHaURYten9bi36V8gt69/zI6B8uTyMJttUma\n"
                                      "ziMr4nBsvfJDT0C0LG9SqsAfPkUMWyHSXxXn7N7S4Yy48k+wyp6FMKN13d/aWqdy\n"
                                      "K4n2W8ux8sHMIQKCAQEAsBXh0HO8eZpanq3Vl0tiHmCpjTsXFUg/hOGrck4s/vaG\n"
                                      "mLiSqCHKX6qfoLrEWUiPfCuqBprAsSebq2c/xkWdwW1UP/6m6dh6REXHbZbTyot4\n"
                                      "JNSPwNSvfu3P6cmLFyao4u6AciO/V18kGXf64XsDZ9gb47oVGtcSmYcsHsVTIyE6\n"
                                      "84+UyO9ogT2CNBN7kHqP5LT5iQsX+YzQcJuEmCp8JO6Az/pkErH15p2uLIHHTXRK\n"
                                      "gGrazVRl4Gt4Qdx4dGk/WcTK6NDPeoi4Wki1DzzK0fJNUDYHItycZDK1bfY+2n+Q\n"
                                      "C5d40kIR4oRdHC0VF94clXuTD/Z7tpgN2vXODP9IlwKCAQAJVxUlmATuqhNRgxNN\n"
                                      "15Cpv+aAfljD2aYyReEADtBNMBjEmES2gi8yzdS9JQTc+02kGx2h2guFXNHDgHxV\n"
                                      "eNrKPq8sMMNB4XXl9AFilBSE7dFDBb2HAOP4fiudHQ5HBFf45bK05vwzse8pi8Om\n"
                                      "3qVt2Q0SjJ2uK1UFTKFKIpNxpttl4H+dbe8VAaCjHwY5E6LCuXPoGFIiB7b0ZkMa\n"
                                      "2uHFv/tomUfU98oCafmxu1bBwf+dW1+iyaLATv+/Vg+LWeZjOqJFck2wYSQRGqqp\n"
                                      "kShu0kOZ9UCUPZvAzdzlD96hHG0kN+arRf/i3ZtLJa5ltkwt7ghyTXI1G4PsOZq2\n"
                                      "8FIhAoIBAEYUf2n6FgIDt5s9ritnuiZC7FgkM1yqA3W8ZwK4MFpM/Wac1umJgUSv\n"
                                      "4JYUnv61zT1rF2FHh/c5v0/paM1deZq5C3XowL+DA65WYzevdp0/AtMNsiTZwPrw\n"
                                      "ZPYz22KcZUzkBUToC0gXuoNUaAoDbmiO7xKkRbAH9wQZcyrP9/WcTR0QgPOzrND7\n"
                                      "DO3y7xOiY9BvYnzzaFhOfcrDanMxPXVpYuTjT57NKwPcr6xQ/mRKKziOzoQ32dAG\n"
                                      "lbcIqvwRwz/T/bnJGTo4Xb64/y6QUFxcZf7NceujB68tK14XSg6mBEtIvrJXz0xq\n"
                                      "x6/mFWYJZTDtHKuWusgCHkmN2LL9iwMCggEAW00XBAIhaMskQh4TiBk8e2n8mBw3\n"
                                      "oqdRPgJ4LWLEdBxYJKoQCffyGW47qZXEvb416r5mq1XwHJlrYUqcf63Lj3ANAt+8\n"
                                      "ifE/FUhwb8jAiji6XTicDJERkM5HrxDA3TS2pbBM2/bIurOzBXGpXNpnMLKbNCur\n"
                                      "bvS5UcsSO5OvRH7JT4E5AwuuNPoiZb3nY/1wlC9VfzAc4tw5W1gMoElkiG1QxsMV\n"
                                      "ip5M37v06Exj2YURmRzIfhOGNgTf9NA91FZb73t94sUqxT0JbiWSA3llXKJn+NYr\n"
                                      "J04EngxPgHewJPwWf50GzLPK62OMKd7O5deTw+HO7qrClcpExZGsakc/cw==\n"
                                      "-----END RSA PRIVATE KEY-----";

static const char RSA_PUBLIC_KEY[] = "-----BEGIN PUBLIC KEY-----\n"
                                     "MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAoWFe7BbX1nWo5oaSv/Jv\n"
                                     "IUCWsk/Vi2q8P0cGkefgN5J7MN7Kfv7lq0hl/1cZcJs81IC+GiC+V3aR2zLBNnJJ\n"
                                     "axa4sqk+hF5DJcD2bF0B80uqPYQUXlQwki/heATnVcke8APuY0kOZykxoD0APAqw\n"
                                     "0z5KDqgt2vA9G6keM6b9bbL+IvxM+yMk1QV0OQLh6Rkz46DyPSoUFWyXiist47PJ\n"
                                     "KNyZAfFZx6vEivzBmqRHKe11W9oD/tN5VTQCH/UTSRfyWq/UUMFVMCksLwT6XoWI\n"
                                     "7F5swgQkSahWkVJ93Qf8cUf1HIZYTMJBYPG4y2NDZ0+ytnH3BNXLMQXg9xbgv6B/\n"
                                     "iaSVScI4CWIpQTAtNKnJwYg2+RhfYBC07iM56c4a+TjbCWgmd11UYc96dbw83uFR\n"
                                     "jKZc3+SC38ITCgMuoDPNBlFJK6u8VfYylGEJolGcauVa6yZKwzsJGr5J/LANz+Zy\n"
                                     "HZmANed+2Hjqxu/H1NGDBdvUGLQbhb/uBJ8oG8iAW5eUyjEJMX0RuncYnBrUjZdE\n"
                                     "Fr0zJd5VkrfFTd26AjGusbiBevATfj83SNa9uK3N3lSNcLNyNXUjmfOU21NWHAk5\n"
                                     "QV3TJb6SCTcqWFaYoyKR7H6zxRcArNuIAMW4KhOl4jdNnTxJllC4tr/gkE+uO1nt\n"
                                     "B9ymLxQBRp8osHjuZpKXr3cCAwEAAQ==\n"
                                     "-----END PUBLIC KEY-----";

static const char JWT[] = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6InNvbWUta2V5LWlkLWhlcmUtMDEyMzQ1In0.eyJpYXQiOjE1ODAzNDAwMzcsImV4cCI6MTU4MDM0MDYzNywic3ViIjoiR29yZG9uIEZyZWVtYW4iLCJpc3MiOiJCbGFjayBNZXNhIiwiYXVkIjoiQWRtaW5pc3RyYXRvciIsImN0eCI6IlVuZm9yc2VlbiBDb25zZXF1ZW5jZXMiLCJhZ2UiOjI3LCJzaXplIjoxLjg1LCJhbGl2ZSI6dHJ1ZSwibnVsbHRlc3QiOm51bGx9.dUaDuY8VLQ-hYLbHvqrVhqPVuC7lFudKdxE10sLo1jSkfCqylMJVYAc0I3zk9RK1caFgrjBToyHBbQP49-oE2fD1F0QyMF0F8v8jYBIIhf-YlXQK8Sv1zE-M_LdX1JpWaZ4YtgrffzcyUawjcYGreuEVuuhX-Abd9c5D6wqkfC5DdnO4UC5dWqAcEgbidMXN2aoY54iX85PU-p2VBxYOkd9kxBZGTqN-eK2XEZ_nwZ32XitSJO8HPxlkgN4TPgc1dTrGQNHwdX71vRU1zK_wUbOnvcbY0puxmKw5yZOk_-twlLO44eTkprwxs0PpPHI-K6YXKwgvrbEYmtWQQ78SOdRQcDxs5DVv65MkmkwHqB5nKQKGg-TtSpbemskeY0IoBGy3hjPGiRDxF2qCGpXdfMMCh2zgFOOq6WBA5BLclWAr9KgWvrDGL7gPklJw2Q9UIP8lGPnnZRIImXHvtRCZ0yxiknemc-KL429JC8kwvnXjbsyU8wYPpT3PNiwJ77bFW3XxJtMODZNhXy4LCgkk3qTtiTEQ5rtjiug-_CjyVTIkS_hXJpKvrYlEF3jt-4WhXWwmfKzhYE3GPJbK8ZO1XSjnb6FSkdAuCh2xHHeg36bf27JH5tnrMzvC6Dsky5jBMTN_ql2h26cOwDDn0IF1_9yc9zW-df1EYfQFyrR4KgY";

int main(void)
{
    struct l8w8jwt_decoding_params params;
    l8w8jwt_decoding_params_init(&params);

    params.alg = L8W8JWT_ALG_RS256;

    params.jwt = (char*)JWT;
    params.jwt_length = strlen(JWT);

    params.verification_key = (unsigned char*)RSA_PUBLIC_KEY;
    params.verification_key_length = strlen(RSA_PUBLIC_KEY);

    params.validate_iss = "Black Mesa";
    params.validate_iss_length = strlen(params.validate_iss);

    params.validate_sub = "Gordon Freeman";
    params.validate_sub_length = strlen(params.validate_sub);

    params.validate_exp = 1;
    params.exp_tolerance_seconds = 60;

    params.validate_iat = 1;
    params.iat_tolerance_seconds = 60;

    enum l8w8jwt_validation_result validation_result;
    int r = l8w8jwt_decode(&params, &validation_result, NULL, NULL);

    printf("\nl8w8jwt_decode_rs256 function returned %s (code %d).\n\nValidation result: \n%d\n", r == L8W8JWT_SUCCESS ? "successfully" : "", r, validation_result);

    char* header = NULL;
    char* payload = NULL;
    uint8_t* signature = NULL;

    size_t header_length = 0;
    size_t payload_length = 0;
    size_t signature_length = 0;

    r = l8w8jwt_decode_raw_no_validation(&params, &header, &header_length, &payload, &payload_length, &signature, &signature_length);

    printf("\nl8w8jwt_decode_raw_no_validation_rs256 function returned %s (code %d).\n\nValidation result: \nN.A.\n\nHeader: %s\n\nPayload: %s\n", r == L8W8JWT_SUCCESS ? "successfully" : "", r, header, payload);

    l8w8jwt_free(header);
    l8w8jwt_free(payload);
    l8w8jwt_free(signature);

    header_length = 0;
    payload_length = 0;
    signature_length = 0;

    r = l8w8jwt_decode_raw(&params, &validation_result, &header, &header_length, &payload, &payload_length, &signature, &signature_length);

    printf("\nl8w8jwt_decode_raw_rs256 function returned %s (code %d).\n\nValidation result: \n%d\n\nHeader: %s\n\nPayload: %s\n", r == L8W8JWT_SUCCESS ? "successfully" : "", r, validation_result, header, payload);

    l8w8jwt_free(header);
    l8w8jwt_free(payload);
    l8w8jwt_free(signature);

    header_length = 0;
    payload_length = 0;
    signature_length = 0;

    return 0;
}
