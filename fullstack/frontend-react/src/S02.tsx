import { useState } from 'react';

const S02 = () => {
  const [username, setUsername] = useState("");

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setUsername(e.target.value);
    console.log("Current username:", e.target.value);
  };

  return (
    <div>
      <p>Username: {username}</p>
      <input type="text" value={username} onChange={handleChange} />
    </div>
  );
};

export default S02;