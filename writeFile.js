import http from 'k6/http';
import { check } from 'k6';

export const options = {
  vus:100,
  iterations: 2000000, 
};

const payload = JSON.stringify({
  // Verify if your backend needs "Text" or "text"
  text: "Hello",
  // Ensure 4 is the correct ID for Uppercase in your C++ Enum
  choice: 4
});

const params = {
  headers: { 'Content-Type': 'application/json' },
};

export default function () {
  // Use localhost if running k6 from your Mac; host.docker.internal if in a container
  //const url = 'http://loadbalancer/api/WordCase/convert';
  const url = 'http://host.docker.internal:80/api/WordCase/convert';
  const res = http.post(url, payload, params);

  check(res, {
    'is status 200': (r) => r.status === 200,
    'transformed to uppercase': (r) => {
      try {
        // Parse the JSON response
        const data = r.json();
        // Check the specific field your API returns
        return data.convertedText === "HELLO";
      } catch (e) {
        return false;
      }
    },
  });
}