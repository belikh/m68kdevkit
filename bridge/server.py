import asyncio
import argparse
import struct
import os
import sys
import httpx
from monocypher.bindings import crypto_aead_lock, crypto_aead_unlock

# Protocol:
# [Length: 4 bytes LE]
# [Nonce: 24 bytes]
# [MAC: 16 bytes]
# [Ciphertext: N bytes]

KEY = b'00000000000000000000000000000000' # Default PSK for dev

class BridgeServer:
    def __init__(self, host='0.0.0.0', port=8080, key=KEY):
        self.host = host
        self.port = port
        self.key = key
        self.send_ctr = 0

    async def handle_client(self, reader, writer):
        addr = writer.get_extra_info('peername')
        print(f"Connected by {addr}")

        try:
            while True:
                # Read Length
                data = await reader.readexactly(4)
                length = struct.unpack('<I', data)[0]

                # Read Frame
                frame = await reader.readexactly(length)

                nonce = frame[:24]
                mac = frame[24:40]
                ciphertext = frame[40:]

                # Decrypt
                plaintext = crypto_aead_unlock(
                    self.key,
                    nonce,
                    mac,
                    ciphertext
                )

                if plaintext is None:
                    print("Decryption failed")
                    break

                # Process Request (Assume simple string: "METHOD URL BODY")
                request_str = plaintext.decode('utf-8')
                print(f"Received: {request_str}")

                parts = request_str.split(' ', 2)
                method = parts[0]
                url = parts[1]
                body = parts[2] if len(parts) > 2 else None

                # Forward Request
                async with httpx.AsyncClient() as client:
                    try:
                        resp = await client.request(method, url, content=body, timeout=10.0)
                        response_data = resp.content
                        print(f"Got response: {resp.status_code}, {len(response_data)} bytes")
                    except Exception as e:
                        response_data = str(e).encode('utf-8')

                # Encrypt Response
                resp_nonce = struct.pack('<Q', self.send_ctr) + b'\x00' * 16
                self.send_ctr += 1

                mac, ciphertext = crypto_aead_lock(self.key, resp_nonce, response_data)

                # Construct Frame
                # Nonce (24) + MAC (16) + Ciphertext (N)
                response_frame = resp_nonce + mac + ciphertext
                response_len = struct.pack('<I', len(response_frame))

                writer.write(response_len + response_frame)
                await writer.drain()

        except asyncio.IncompleteReadError:
            pass
        except Exception as e:
            print(f"Error: {e}")
        finally:
            writer.close()

    async def start(self):
        server = await asyncio.start_server(
            self.handle_client, self.host, self.port)

        print(f'Serving on {self.host}:{self.port}')
        async with server:
            await server.serve_forever()

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', type=int, default=8080)
    args = parser.parse_args()

    # Try to install monocypher binding if missing (simplified for this context)
    # real impl would rely on requirements.txt
    try:
        from monocypher.bindings import crypto_aead_lock
    except ImportError:
        print("Please install monocypher: pip install monocypher-py")
        # For the sake of this environment without internet to pip install easily inside the python run
        # I will mock the crypto if import fails just to allow the script to be valid syntax,
        # but the plan already installed requirements.
        sys.exit(1)

    bs = BridgeServer(port=args.port)
    asyncio.run(bs.start())
