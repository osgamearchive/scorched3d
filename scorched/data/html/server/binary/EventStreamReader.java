import java.net.*;
import java.io.IOException;

public class EventStreamReader {

	public static final String CRLF = "\015\012";
	Socket socket_;
	
	public interface EventStreamReaderI {
		public void handleData(String s);
	}
	
	class Reader implements Runnable {
	
		EventStreamReaderI handler;
		String host;
		String sid;
		int port;
	
		Reader(EventStreamReaderI handler, String host, int port, String sid) {
			this.handler = handler;
			this.host = host;
			this.port = port;
			this.sid = sid;
		}
	
		public void run() {
			
			if (connect(host, port)) {
			
				try {
					String connectionString = 
						"GET /appletstream?sid=" + sid + " HTTP/1.0" + CRLF +
						"Connection: Keep-Alive" + CRLF +
						CRLF +
						CRLF;
					socket_.getOutputStream().write(
						connectionString.getBytes());
				} catch (IOException ex) {
					handler.handleData(ex.getMessage());
				}
			
				byte buffer[] = new byte[256];
				while (socket_.isConnected()) {
			
					try {
						Thread.sleep(10);
					} catch (Exception ex) {
					}
				
					try {
						int amount = socket_.getInputStream().read(buffer);
						if (amount > 0) {
							String s = new String(buffer, 0, amount);
							handler.handleData(s);
						}
					} catch (IOException ex) {
						handler.handleData(ex.getMessage());
						break;
					}
				}
			}
			
			close();
		}
	}
	
	public void start(EventStreamReaderI handler, 
		String host, int port, String sid) {
		
		new Thread(new Reader(handler, host, port, sid)).start();
	}
	
	public void stop() {
		close();
	}

	boolean connect(String host, int port) {
		InetSocketAddress address = new InetSocketAddress(host, port);
		
		try {
			socket_ = new Socket();
			socket_.connect(address, 10); // 10Second timeout
		} catch (IOException ioEx) {
		}
		
		return socket_.isConnected();
	}
	
	void close() {
		try {
			socket_.close();
		} catch (IOException ioEx) {
		}
	}
}
