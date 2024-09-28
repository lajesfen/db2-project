import { useEffect } from 'react';

interface NotificationProps {
  message: string;
  duration?: number;
  onClose: () => void;
}

export default function Notification({ message, duration = 3000, onClose }: NotificationProps) {
  useEffect(() => {
    const timer = setTimeout(() => {
      onClose();
    }, duration);

    return () => clearTimeout(timer);
  }, [duration, onClose]);

  return (
    <div className="flex text-red-500 italic items-center">
      {message}
    </div>
  );
};